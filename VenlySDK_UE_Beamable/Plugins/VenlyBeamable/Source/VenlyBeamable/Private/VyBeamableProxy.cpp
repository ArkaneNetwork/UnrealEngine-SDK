// Fill out your copyright notice in the Description page of Project Settings.


#include "VyBeamableProxy.h"
#include "Utils/VyResponseUtils.h"
#include "VyCore.h"

DEFINE_LOG_CATEGORY(LogVenlyBeamable);

void UVyBeamableProxy::ExecuteRequest(FVyRequestContext& Context, EVyEnvironment Env, const FVyOnComplete_ResponseContext& OnComplete)
{
	//Make sure either CPP or BP delegate is bound
	if (!OnExecuteRequest.IsBound() && !OnExecuteRequestHandlerBP.IsBound())
	{
		auto response = VyResponseUtils::CreateResponseContext_Failure(TEXT("BEAMABLE_PROVIDER"), TEXT("There is no Microservice bound. Make sure to Bind through CPP using UVyBeamableProxy::OnExecuteRequest, or through Blueprints using UVyBeamableProxy::BindExecuteRequest(...)"));
		auto _ = OnComplete.ExecuteIfBound(response);
		return;
	}

	//Serialize Request Data
	auto requestData = Context.ToRequestData(Env);
	FString requestDataString{};
	if (!VyJsonUtils::UStructToJsonObjectString(requestData, requestDataString))
	{
		auto response = VyResponseUtils::CreateResponseContext_Failure(TEXT("BEAMABLE_PROVIDER"), TEXT("Failed to serialize Microservice request data (FVyRequestData)"));
		auto _ = OnComplete.ExecuteIfBound(response);
		return;
	}

	bool delegateCalled = false;
	TrackedRequests.Add(Context.Id, OnComplete);

	//CPP flow
	if (OnExecuteRequest.IsBound())
	{
		//OnExecuteRequestComplete Delegate
		FVyOnExecuteProviderRequestComplete onExecuteRequestComplete{};
		onExecuteRequestComplete.BindLambda([this, Context](const FVyBeamableMicroserviceResponse& Response) {
			SignalRequestComplete(Context.Id, Response);
			});

		delegateCalled = OnExecuteRequest.ExecuteIfBound(requestDataString, onExecuteRequestComplete);
	}

	//BP flow
	else if(OnExecuteRequestHandlerBP.IsBound())
	{
		delegateCalled = OnExecuteRequestHandlerBP.ExecuteIfBound(requestDataString, Context.Id);
	}

	if (!delegateCalled)
	{
		TrackedRequests.Remove(Context.Id);
		auto response = VyResponseUtils::CreateResponseContext_Failure(TEXT("BEAMABLE_PROVIDER"), TEXT("Execution of the Beamable Proxy delegate failed"));
		auto _ = OnComplete.ExecuteIfBound(response);
	}
}

void UVyBeamableProxy::SignalRequestSuccess(int64 RequestId, const FString& ResponseData)
{
	SignalRequestComplete(RequestId, FVyBeamableMicroserviceResponse{ ResponseData });
}

void UVyBeamableProxy::SignalRequestError(int64 RequestId, const FBeamErrorResponse& ErrorData)
{
	SignalRequestComplete(RequestId, FVyBeamableMicroserviceResponse{ ErrorData });
}

void UVyBeamableProxy::SignalRequestComplete(int64 RequestId, const FVyBeamableMicroserviceResponse& ExecuteReponse)
{
	if (!TrackedRequests.Contains(RequestId))
	{
		UE_LOG(LogVenlyBeamable, Warning, TEXT("RequestID \'%i\' was not found. Failed to signal execute completion..."), RequestId)
		return;
	}

	//Remove Delegate from MAP
	FVyOnComplete_ResponseContext OnComplete{};
	TrackedRequests.RemoveAndCopyValue(RequestId, OnComplete);

	if (ExecuteReponse.Success)
	{
		FVyServerResponse ServerResponse{};
		FString DecodedResult{};
		FBase64::Decode(ExecuteReponse.Result, DecodedResult); //Decode top-level

		if (VyJsonUtils::JsonObjectStringToUStruct(DecodedResult, &ServerResponse))
		{
			//if (ServerResponse.IsDataEncoded) {
			//	FBase64::Decode(ServerResponse.Data, ServerResponse.Data); //Decode data-level
			//	ServerResponse.IsDataEncoded = false;
			//}

			auto response = VyResponseUtils::CreateResponseContext_ServerResponse(ServerResponse, TEXT("BEAMABLE_PROVIDER::MICROSERVICE"));
			auto _ = OnComplete.ExecuteIfBound(response);
		}
		else
		{
			auto response = VyResponseUtils::CreateResponseContext_Failure(TEXT("BEAMABLE_PROVIDER"), FString::Printf(TEXT("Failed to parse the server response. [%s]"), *DecodedResult));
			auto _ = OnComplete.ExecuteIfBound(response);
		}
	}
	else
	{
		auto response = VyResponseUtils::CreateResponseContext_Failure(ExecuteReponse.Error.error, ExecuteReponse.Error.message);
		auto _ = OnComplete.ExecuteIfBound(response);
	}
}

void UVyBeamableProxy::BindExecuteRequest(const FVyOnExecuteProviderRequestBP& handler)
{
	OnExecuteRequestHandlerBP = handler;
}