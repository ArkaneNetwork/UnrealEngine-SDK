﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestTracker/BeamRequestTracker.h"
#include "BeamLogging.h"


void UBeamRequestTracker::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	WaitHandleId = new long();
	*WaitHandleId = -1;

	OperationHandleId = new long();
	*OperationHandleId = -1;

	Backend = Cast<UBeamBackend>(Collection.InitializeDependency(UBeamBackend::StaticClass()));

	// Set up the RequestId-related cleanup
	TickOnRequestIdCompletedDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UBeamRequestTracker, CheckAndCompleteWaitHandles));
	Backend->TickOnRequestIdCompletedDelegates.Add(TickOnRequestIdCompletedDelegate);

	// Set up the RequestId-related cleanup
	TickOnBackendCleanUpDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UBeamRequestTracker, HandleBackendCleanUp));
	Backend->TickOnBackendCleanUpDelegates.Add(TickOnBackendCleanUpDelegate);


	TickCleanUpRequestTracker = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](const float)
	{
		// Clean up Completed wait handles
		CleanUpWaitHandles();

		// Clean up Completed Operations
		CleanUpOperations();

		return true;
	}), GetDefault<UBeamCoreSettings>()->RequestTrackerCleanUpInterval);
}

void UBeamRequestTracker::Deinitialize()
{
	Super::Deinitialize();

	// Clean up backend delegates
	Backend->TickOnRequestIdCompletedDelegates.Remove(TickOnRequestIdCompletedDelegate);
	Backend->TickOnBackendCleanUpDelegates.Remove(TickOnBackendCleanUpDelegate);
	TickOnBackendCleanUpDelegate.Clear();
	TickOnRequestIdCompletedDelegate.Clear();

	FTSTicker::GetCoreTicker().RemoveTicker(TickCleanUpRequestTracker);

	delete WaitHandleId;
	delete OperationHandleId;
}

void UBeamRequestTracker::GatherRequestIdsFromWaitHandle(const FBeamWaitHandle Handle, TArray<FBeamRequestId>& DependedOnRequests, TArray<FBeamOperationId>& DependedOnOperations) const
{
	// We get all the Request Ids that were awaited on directly		
	ActiveRequestsForWaitHandles.MultiFind(Handle, DependedOnRequests);

	// Then, we get all the operations that we are waiting on
	TArray<FBeamOperationId> Deps;
	ActiveOperationsForWaitHandles.MultiFind(Handle, Deps);
	for (const auto& Op : Deps)
	{
		check(ActiveOperations.Contains(Op))
		DependedOnOperations.Add(Op);

		const auto State = ActiveOperationState.FindChecked(Op);
		for (const auto& Req : State.DependentRequests)
		{
			DependedOnRequests.AddUnique(Req);
		}
	}

	// Finally, we recursively get all request ids from any wait handles this wait handle depends on.
	TArray<FBeamWaitHandleId> DependedOnWaitHandles;
	ActiveWaitHandlesForWaitHandles.MultiFind(Handle, DependedOnWaitHandles);
	for (const auto& DependentWaitHandleId : DependedOnWaitHandles)
	{
		const auto DependedHandle = ActiveWaitHandles.FindByPredicate([DependentWaitHandleId](const FBeamWaitHandle& wh) { return wh.WaitHandleId == DependentWaitHandleId; });
		ensureAlwaysMsgf(DependedHandle, TEXT("Somehow you are depending on a wait handle that doesn't exist... This should be impossible"));
		GatherRequestIdsFromWaitHandle(*DependedHandle, DependedOnRequests, DependedOnOperations);
	}
}

void UBeamRequestTracker::CheckAndCompleteWaitHandles(int64)
{
	bool bDidCompleteAtLeastOneWait = false;
	// Go through all active wait handles and figure out which of them have been completed.
	// If they have, we run their register FOnWaitCompleted callback.
	for (int i = ActiveWaitHandles.Num() - 1; i >= 0; --i)
	{
		auto& ActiveWaitHandle = ActiveWaitHandles[i];
		if (ActiveWaitHandle.Status == AS_Completed)
		{
			UE_LOG(LogBeamRequestTracker, Verbose, TEXT("Beamable WaitHandle | WaitHandle is already completed and awaiting clean up. WAIT_HANDLE=%llu"), ActiveWaitHandle.WaitHandleId);
			continue;
		}

		// We get all the Request Ids that were awaited on directly
		TArray<FBeamRequestId> DependedOnRequests;
		TArray<FBeamOperationId> DependedOnOperations;
		GatherRequestIdsFromWaitHandle(ActiveWaitHandle, DependedOnRequests, DependedOnOperations);

		// We also sort the dependencies so that they are returned in the order they were made.
		DependedOnRequests.Sort();

		// Handle the All case: Basically, all the dependency nodes need to be completed in order for us to fire off the wait event.
		if (ActiveWaitHandle.Mode == All)
		{
			auto bAreAllDepsDone = true;

			// If operations we depend on are still running, we are not done.
			for (const auto& Op : DependedOnOperations)
			{
				check(ActiveOperations.Contains(Op))
				const auto State = ActiveOperationState.FindChecked(Op);
				bAreAllDepsDone &= State.Status != FBeamOperationState::ONGOING;
			}

			// If requests (from within each operation or individually made) are still running, we are not done.
			for (const auto& DependedOnId : DependedOnRequests)
			{
				const auto& DependedOnCtx = Backend->InFlightRequestContexts.FindRef(DependedOnId);
				bAreAllDepsDone &= DependedOnCtx.BeamStatus == AS_Completed;
			}

			// Track if we completed at least one wait dependency.
			bDidCompleteAtLeastOneWait |= bAreAllDepsDone;

			// If all dependencies are done, let's call the WaitCompleted callback with all the dependencies correctly set up!
			if (bAreAllDepsDone)
			{
				TArray<FBeamRequestContext> Contexts;
				TArray<TScriptInterface<IBeamBaseRequestInterface>> RequestData;
				TArray<UObject*> ResponseBodies;
				TArray<FBeamErrorResponse> ResponseErrors;

				// String builder so we can log all the ids
				FString IdList;
				IdList.Reserve(DependedOnRequests.Num() * 2);

				// Go through ids
				for (const auto& DependedOnId : DependedOnRequests)
				{
					const auto DependedOnContext = Backend->InFlightRequestContexts.FindRef(DependedOnId);

					Contexts.Add(DependedOnContext);
					RequestData.Add(Backend->InFlightRequestData.FindRef(DependedOnContext));
					ResponseBodies.Add(Backend->InFlightResponseBodyData.FindRef(DependedOnContext));
					ResponseErrors.Add(Backend->InFlightResponseErrorData.FindRef(DependedOnContext));
					IdList += FString::Printf(TEXT("%llu, "), DependedOnId);
				}

				UE_LOG(LogBeamRequestTracker, Verbose, TEXT("Beamable WaitHandles | Completed WAIT_HANDLE=%llu, REQUEST_IDS=[%s]"), ActiveWaitHandle.WaitHandleId, *IdList);

				ActiveWaitHandle.Status = AS_Completed;

				if (const auto OnComplete = ActiveWaitHandleCodeCallbacks.Find(ActiveWaitHandle))
					auto _ = OnComplete->ExecuteIfBound(Contexts, RequestData, ResponseBodies, ResponseErrors);

				if (const auto OnComplete = ActiveWaitHandleCallbacks.Find(ActiveWaitHandle))
					auto _ = OnComplete->ExecuteIfBound(Contexts, RequestData, ResponseBodies, ResponseErrors);
			}
		}
	}

	// We re-run the check if at least one wait handle was completed.
	if (bDidCompleteAtLeastOneWait) CheckAndCompleteWaitHandles(-1);
}


void UBeamRequestTracker::CleanUpWaitHandles()
{
	// Go through all active wait handles and figure out which of them have been completed.
	// If they have, we run their register FOnWaitCompleted callback.
	for (int i = ActiveWaitHandles.Num() - 1; i >= 0; --i)
	{
		auto ActiveWaitHandle = ActiveWaitHandles[i];
		if (ActiveWaitHandle.Status != AS_InFlight)
		{
			// Notify the Operations that this wait handle was completed so that any Operation that was dependent on it can clean itself up correctly in CleanUpOperations().
			TArray<FBeamRequestId> DependedOnRequests;
			TArray<FBeamOperationId> DependedOnOperations;
			GatherRequestIdsFromWaitHandle(ActiveWaitHandle, DependedOnRequests, DependedOnOperations);
			// First, we make sure remove this completed wait handle from the map of Operation->WaitHandles that depend on it.
			for (const auto& DependedOnOperation : DependedOnOperations)
			{
				WaitHandlesForActiveOperations.Remove(DependedOnOperation, ActiveWaitHandle);
			}

			// Clear the handle from active ones
			UE_LOG(LogBeamRequestTracker, Verbose, TEXT("Beamable CleanUp | Cleaning Up Data associated with WaitHandle. WAIT_HANDLE_ID=%llu"), ActiveWaitHandle.WaitHandleId);
			ActiveWaitHandles.RemoveAt(i);

			// Clear the handle from the map of dependencies for them.
			ActiveRequestsForWaitHandles.Remove(ActiveWaitHandle);
			ActiveOperationsForWaitHandles.Remove(ActiveWaitHandle);
			ActiveWaitHandlesForWaitHandles.Remove(ActiveWaitHandle);

			// Clear the callback associated with the handle
			ActiveWaitHandleCallbacks.Remove(ActiveWaitHandle);
		}
	}
}


void UBeamRequestTracker::CleanUpOperations()
{
	// Go through all active wait handles and figure out which of them have been completed.
	// If they have, we run their register FOnWaitCompleted callback.
	for (int i = ActiveOperations.Num() - 1; i >= 0; --i)
	{
		const auto OpId = ActiveOperations[i];

		// We skip all non-completed operations
		if (const auto& Op = ActiveOperationState[OpId]; Op.Status == FBeamOperationState::ONGOING)
		{
			UE_LOG(LogBeamRequestTracker, Verbose, TEXT("Beamable Operations | Operation is ongoing so we won't clean it up. OP_ID=%llu"), OpId.OperationId);
			continue;
		}

		// If they are completed, we need to figure out if anyone depends on it.
		const auto bIsDependedOn = WaitHandlesForActiveOperations.Contains(OpId);
		auto bDependenciesAreFinished = !bIsDependedOn;
		if (bIsDependedOn)
		{
			TArray<FBeamWaitHandle> WaitHandles;
			WaitHandlesForActiveOperations.MultiFind(OpId, WaitHandles);
			for (const auto WaitHandle : WaitHandles)
			{
				bDependenciesAreFinished &= WaitHandle.Status != AS_Completed;
			}
		}

		// If the operation is done and no wait handle depends on it, just clean it up.
		if (bDependenciesAreFinished)
		{
			UE_LOG(LogBeamRequestTracker, Verbose, TEXT("Beamable CleanUp | Cleaning Up Operation. OP_ID=%llu"), OpId.OperationId);
			ActiveOperations.RemoveAt(i);
			ActiveOperationState.Remove(OpId);
			ActiveOperationEventHandlers.Remove(OpId);
			WaitHandlesForActiveOperations.Remove(OpId);
		}
	}
}

void UBeamRequestTracker::HandleBackendCleanUp(TArray<int64>& OutUsingRequestIds)
{
	// Finally, we go through the current wait handles and operations to find the all request ids that  
	TArray<FBeamRequestId> RequestIdsHelper;
	RequestIdsHelper.Reserve(16);

	TArray<FBeamOperationId> IdsHelper;
	TArray<FBeamOperationId> DependencyOperationIds;
	IdsHelper.Reserve(4);
	DependencyOperationIds.Reserve(16);

	// For every active wait handle add their Request Ids so that BeamBackend knows not to clean them up yet.
	for (const auto& ActiveWaitHandle : ActiveWaitHandles)
	{
		if (ActiveWaitHandle.Status == AS_InFlight)
		{
			ActiveRequestsForWaitHandles.MultiFind(ActiveWaitHandle, RequestIdsHelper);
			for (const auto& ReqId : RequestIdsHelper) OutUsingRequestIds.AddUnique(ReqId);

			ActiveOperationsForWaitHandles.MultiFind(ActiveWaitHandle, IdsHelper);
			DependencyOperationIds.Append(IdsHelper);

			RequestIdsHelper.Reset();
			IdsHelper.Reset();
		}
	}

	// For every active operation add their Request Ids so that BeamBackend knows not to clean them up yet.
	for (const auto& ActiveOperationId : ActiveOperations)
	{
		const auto State = ActiveOperationState.FindChecked(ActiveOperationId);
		if (State.Status == FBeamOperationState::ONGOING || DependencyOperationIds.Contains(ActiveOperationId.OperationId))
		{
			for (const auto& Req : State.DependentRequests)
				OutUsingRequestIds.AddUnique(Req);
		}
	}
}

FBeamWaitHandle UBeamRequestTracker::WaitAll(const TArray<FBeamRequestContext>& RequestContexts, const TArray<FBeamOperationHandle>& Operations, const TArray<FBeamWaitHandle>& Waits,
                                             FOnWaitComplete OnComplete)
{
	// Ensures we get a valid Next Id even if requests get made from multiple threads.
	const auto NextId = _InterlockedIncrement(WaitHandleId);
	const auto WaitHandle = FBeamWaitHandle{NextId, Backend, All, AS_InFlight};

	ActiveWaitHandles.Add(WaitHandle);

	auto bDepExists = true;
	for (const auto& Context : RequestContexts)
	{
		bDepExists &= Backend->InFlightRequests.Contains(Context.RequestId);
		ensureAlwaysMsgf(bDepExists, TEXT("Added Request Dependency to WaitHandle does not exist. OWNER_WAIT_HANDLE=%lld, REQUEST_ID=%lld"),
		                 WaitHandle.WaitHandleId, Context.RequestId);
		UE_LOG(LogBeamRequestTracker, Verbose, TEXT("Adding Request Dependency to WaitHandle. OWNER_WAIT_HANDLE=%lld, REQUEST_ID=%lld"), WaitHandle.WaitHandleId, Context.RequestId);
		ActiveRequestsForWaitHandles.AddUnique(WaitHandle, Context.RequestId);
	}

	for (const auto& Operation : Operations)
	{
		bDepExists &= ActiveOperations.Contains(Operation);
		ensureAlwaysMsgf(bDepExists, TEXT("Added Operation Dependency to WaitHandle does not exist. OWNER_WAIT_HANDLE=%lld, OPERATION_ID=%lld"),
		                 WaitHandle.WaitHandleId, Operation.OperationId);

		UE_LOG(LogBeamRequestTracker, Verbose, TEXT("Adding Operation Dependency to WaitHandle. OWNER_WAIT_HANDLE=%lld, OPERATION_ID=%lld"), WaitHandle.WaitHandleId, Operation.OperationId);
		ActiveOperationsForWaitHandles.AddUnique(WaitHandle, Operation.OperationId);
		WaitHandlesForActiveOperations.Add(Operation, WaitHandle);
	}

	for (const auto& WaitDep : Waits)
	{
		bDepExists &= ActiveWaitHandles.Contains(WaitDep);
		ensureAlwaysMsgf(bDepExists, TEXT("Added WaitHandle Dependency to WaitHandle does not exist. OWNER_WAIT_HANDLE=%lld, WAIT_HANDLE_ID=%lld"),
		                 WaitHandle.WaitHandleId, WaitDep.WaitHandleId);

		UE_LOG(LogBeamRequestTracker, Verbose, TEXT("Adding WaitHandle Dependency to WaitHandle. OWNER_WAIT_HANDLE=%lld, WAIT_HANDLE_ID=%lld"), WaitHandle.WaitHandleId, WaitDep.WaitHandleId);
		ActiveWaitHandlesForWaitHandles.AddUnique(WaitHandle, WaitDep.WaitHandleId);
	}
	ActiveWaitHandleCallbacks.Add(WaitHandle, OnComplete);

	// Handle extremely unlikely edge-case of all dependencies of a wait being completed by the time you call the wait.
	// Just going through this case typically means you're making some dangerous architecture decisions and you should probably re-think them.
	// We only run this section if all the dependencies exist. 
	if (bDepExists)
	{
		CheckAndCompleteWaitHandles(-1);
	}

	return WaitHandle;
}

FBeamWaitHandle UBeamRequestTracker::CPP_WaitAll(const TArray<FBeamRequestContext>& RequestContexts, const TArray<FBeamOperationHandle>& Operations, const TArray<FBeamWaitHandle>& Waits,
                                                 FOnWaitCompleteCode OnCompleteCode)
{
	// Ensures we get a valid Next Id even if requests get made from multiple threads.
	const auto NextId = _InterlockedIncrement(WaitHandleId);
	const auto WaitHandle = FBeamWaitHandle{NextId, Backend, All, AS_InFlight};

	ActiveWaitHandles.Add(WaitHandle);

	auto bDepExists = true;
	for (const auto& Context : RequestContexts)
	{
		bDepExists &= Backend->InFlightRequests.Contains(Context.RequestId);
		ensureAlwaysMsgf(bDepExists, TEXT("Added Request Dependency to WaitHandle does not exist. OWNER_WAIT_HANDLE=%lld, REQUEST_ID=%lld"),
		                 WaitHandle.WaitHandleId, Context.RequestId);
		UE_LOG(LogBeamRequestTracker, Verbose, TEXT("Adding Request Dependency to WaitHandle. OWNER_WAIT_HANDLE=%lld, REQUEST_ID=%lld"), WaitHandle.WaitHandleId, Context.RequestId);
		ActiveRequestsForWaitHandles.AddUnique(WaitHandle, Context.RequestId);
	}

	for (const auto& Operation : Operations)
	{
		bDepExists &= ActiveOperations.Contains(Operation);
		ensureAlwaysMsgf(bDepExists, TEXT("Added Operation Dependency to WaitHandle does not exist. OWNER_WAIT_HANDLE=%lld, OPERATION_ID=%lld"),
		                 WaitHandle.WaitHandleId, Operation.OperationId);

		UE_LOG(LogBeamRequestTracker, Verbose, TEXT("Adding Operation Dependency to WaitHandle. OWNER_WAIT_HANDLE=%lld, OPERATION_ID=%lld"), WaitHandle.WaitHandleId, Operation.OperationId);
		ActiveOperationsForWaitHandles.AddUnique(WaitHandle, Operation.OperationId);
		WaitHandlesForActiveOperations.Add(Operation, WaitHandle);
	}

	for (const auto& WaitDep : Waits)
	{
		bDepExists &= ActiveWaitHandles.Contains(WaitDep);
		ensureAlwaysMsgf(bDepExists, TEXT("Added WaitHandle Dependency to WaitHandle does not exist. OWNER_WAIT_HANDLE=%lld, WAIT_HANDLE_ID=%lld"),
		                 WaitHandle.WaitHandleId, WaitDep.WaitHandleId);

		UE_LOG(LogBeamRequestTracker, Verbose, TEXT("Adding WaitHandle Dependency to WaitHandle. OWNER_WAIT_HANDLE=%lld, WAIT_HANDLE_ID=%lld"), WaitHandle.WaitHandleId, WaitDep.WaitHandleId);
		ActiveWaitHandlesForWaitHandles.AddUnique(WaitHandle, WaitDep.WaitHandleId);
	}
	ActiveWaitHandleCodeCallbacks.Add(WaitHandle, OnCompleteCode);

	// Handle extremely unlikely edge-case of all dependencies of a wait being completed by the time you call the wait.
	// Just going through this case typically means you're making some dangerous architecture decisions and you should probably re-think them.
	// We only run this section if all the dependencies exist. 
	if (bDepExists)
	{
		CheckAndCompleteWaitHandles(-1);
	}

	return WaitHandle;
}

FBeamOperationHandle UBeamRequestTracker::BeginOperation(const TArray<FUserSlot>& Participants, const FString& CallingSystem, FBeamOperationEventHandler OnEvent, int MaxRequestsInOperation)
{
	// Ensures we get a valid Next Id even if requests get made from multiple threads.	
	const auto NextId = _InterlockedIncrement(OperationHandleId);
	const auto OperationHandle = FBeamOperationHandle{NextId};
	ActiveOperations.Add(OperationHandle);

	// Initialize it's state.
	auto State = FBeamOperationState{EBeamOperationEventType::OET_NONE, CallingSystem, {}, Participants, MaxRequestsInOperation};
	ActiveOperationState.Add(OperationHandle, State);
	ActiveOperationEventHandlers.Add(OperationHandle, OnEvent);

	return OperationHandle;
}

FBeamOperationHandle UBeamRequestTracker::CPP_BeginOperation(const TArray<FUserSlot>& Participants, const FString& CallingSystem, FBeamOperationEventHandlerCode OnEvent, int MaxRequestsInOperation)
{
	// Ensures we get a valid Next Id even if requests get made from multiple threads.
	const auto NextId = _InterlockedIncrement(OperationHandleId);
	const auto OperationHandle = FBeamOperationHandle{NextId};
	ActiveOperations.Add(OperationHandle);

	// Initialize it's state.
	auto State = FBeamOperationState{EBeamOperationEventType::OET_NONE, CallingSystem, {}, Participants, MaxRequestsInOperation};
	ActiveOperationState.Add(OperationHandle, State);
	ActiveOperationEventHandlersCode.Add(OperationHandle, OnEvent);

	return OperationHandle;
}

void UBeamRequestTracker::AddRequestToOperation(const FBeamOperationHandle& Op, int64 RequestId)
{
	auto& OperationState = ActiveOperationState.FindChecked(Op);
	ensureAlwaysMsgf(OperationState.MaximumRequestCount == -1 || OperationState.DependentRequests.Num() - 1 < OperationState.MaximumRequestCount, TEXT("Unexpected Request added to Operation"));
	ensureAlwaysMsgf(!OperationState.Status, TEXT("Can't add a request to a Cancelled or Done Operation"));

	const auto InFlightRequest = Backend->InFlightRequests.FindChecked(RequestId);
	const auto InFlightRequestStatus = InFlightRequest->GetStatus();
	ensureAlwaysMsgf(InFlightRequestStatus == EHttpRequestStatus::NotStarted ||
	                 InFlightRequestStatus == EHttpRequestStatus::Processing,
	                 TEXT("Cannot depend on a request that is already done!"));

	OperationState.DependentRequests.Add(RequestId);
}

void UBeamRequestTracker::AddRequestToOperation(const FBeamOperationHandle& Op, FBeamRequestContext RequestContext)
{
	AddRequestToOperation(Op, RequestContext.RequestId);
}

void UBeamRequestTracker::TriggerOperationEvent(const FBeamOperationHandle& Op, const EBeamOperationEventType Type, uint8 SubEvent, const FString& EventData, const int64& RequestId)
{
	const auto& OperationState = ActiveOperationState.FindChecked(Op);

	// If we have the default one, let's pass along '-1000' only if there were no dependent requests. If there, were assume it's a sequential chain and we are being called in the latest request's handler.
	// As such, we pass in the last dependent request id added.
	const auto ReqId = RequestId == DEFAULT_REQUEST_ID ? (OperationState.DependentRequests.Num() > 0 ? OperationState.DependentRequests.Last() : -1) : RequestId;
	TriggerOperationEventFull(Op, Type, SubEvent, OperationState.DependentUserSlots, EventData, OperationState.CallingSystem, ReqId);
}

void UBeamRequestTracker::TriggerOperationEventFull(const FBeamOperationHandle& Op, const EBeamOperationEventType Type, uint8 SubEvent, const TArray<FUserSlot>& UserSlots, const FString& EventData,
                                                    const FString& CallingSystem, const int64 RequestId)
{
	/*checkf(ActiveOperationState.FindRef(Op).Status <= 0, TEXT("Cannot trigger an operation event after it's being completed! %s, %d"), *ActiveOperationState.FindRef(Op).CallingSystem,
	       ActiveOperationState.FindRef(Op).Status);*/

	const auto& OperationEventHandlerCode = ActiveOperationEventHandlersCode.Find(Op);
	const auto& OperationEventHandler = ActiveOperationEventHandlers.Find(Op);

	const FBeamOperationEvent Result{Type, RequestId, SubEvent, CallingSystem, EventData, Op};

	TArray<FString> SlotsStr;
	for (const auto& UserSlot : UserSlots)
		SlotsStr.Add(UserSlot.Name);
	const auto SlotsJoinedStr = FString::Join(SlotsStr, TEXT(","));

	if (OperationEventHandlerCode && OperationEventHandlerCode->ExecuteIfBound(UserSlots, Result))
	{
		UE_LOG(LogBeamRequestTracker, Verbose, TEXT("Called CPP Handler for Operation Event: OPERATION_ID=%lld SLOTS=[%s], EVENT_TYPE=%s, SUB_EVENT=%c, CALLING_SYSTEM=%s, DATA=%s"),
		       Op.OperationId,
		       *SlotsJoinedStr,
		       *StaticEnum<EBeamOperationEventType>()->GetNameStringByValue(static_cast<uint8>(Type)),
		       SubEvent,
		       *CallingSystem,
		       *EventData);
	}

	if (OperationEventHandler && OperationEventHandler->ExecuteIfBound(UserSlots, Result))
	{
		UE_LOG(LogBeamRequestTracker, Verbose, TEXT("Called Dynamic Handler for Operation Event: OPERATION_ID=%lld SLOTS=[%s], EVENT_TYPE=%s, SUB_EVENT=%c, CALLING_SYSTEM=%s, DATA=%s"),
		       Op.OperationId,
		       *SlotsJoinedStr,
		       *StaticEnum<EBeamOperationEventType>()->GetNameStringByValue(static_cast<uint8>(Type)),
		       SubEvent,
		       *CallingSystem,
		       *EventData);
	}

	if (SubEvent == Final)
	{
		auto& OperationState = ActiveOperationState.FindChecked(Op);
		OperationState.Status = FBeamOperationState::COMPLETE;
		CheckAndCompleteWaitHandles(-1);
	}
}

void UBeamRequestTracker::TriggerOperationSuccess(const FBeamOperationHandle& Op, const FString& EventData, const int64& RequestId)
{
	TriggerOperationEvent(Op, EBeamOperationEventType::OET_SUCCESS, Final, EventData, RequestId);
}

void UBeamRequestTracker::TriggerOperationError(const FBeamOperationHandle& Op, const FString& EventData, const int64& RequestId)
{
	TriggerOperationEvent(Op, EBeamOperationEventType::OET_ERROR, Final, EventData, RequestId);
}

void UBeamRequestTracker::TriggerOperationCancelled(const FBeamOperationHandle& Op, const FString& EventData, const int64& RequestId)
{
	TriggerOperationEvent(Op, EBeamOperationEventType::OET_CANCELLED, Final, EventData, RequestId);
}
