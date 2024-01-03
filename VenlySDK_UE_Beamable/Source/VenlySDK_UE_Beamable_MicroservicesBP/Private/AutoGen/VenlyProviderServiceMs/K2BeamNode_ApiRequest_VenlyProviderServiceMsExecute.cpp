

#include "VenlySDK_UE_Beamable_MicroservicesBP\Public\AutoGen/VenlyProviderServiceMs/K2BeamNode_ApiRequest_VenlyProviderServiceMsExecute.h"

#include "BeamK2.h"

#include "VenlySDK_UE_Beamable\AutoGen/SubSystems/BeamVenlyProviderServiceMsApi.h"
#include "VenlySDK_UE_Beamable\AutoGen/SubSystems/VenlyProviderServiceMs/VenlyProviderServiceMsExecuteRequest.h"
#include "VenlySDK_UE_Beamable\AutoGen/VenlyProviderServiceMsExecuteResponse.h"

#define LOCTEXT_NAMESPACE "K2BeamNode_ApiRequest_VenlyProviderServiceMsExecute"

using namespace BeamK2;

FName UK2BeamNode_ApiRequest_VenlyProviderServiceMsExecute::GetSelfFunctionName() const
{
	return GET_FUNCTION_NAME_CHECKED(UBeamVenlyProviderServiceMsApi, GetSelf);
}

FName UK2BeamNode_ApiRequest_VenlyProviderServiceMsExecute::GetRequestFunctionName() const
{
	return GET_FUNCTION_NAME_CHECKED(UBeamVenlyProviderServiceMsApi, Execute);
}

FName UK2BeamNode_ApiRequest_VenlyProviderServiceMsExecute::GetMakeFunctionName() const
{
	return GET_FUNCTION_NAME_CHECKED(UVenlyProviderServiceMsExecuteRequest, Make);
}

FString UK2BeamNode_ApiRequest_VenlyProviderServiceMsExecute::GetServiceName() const
{
	return TEXT("VenlyProviderServiceMs");
}

FString UK2BeamNode_ApiRequest_VenlyProviderServiceMsExecute::GetEndpointName() const
{
	return TEXT("Execute");
}

UClass* UK2BeamNode_ApiRequest_VenlyProviderServiceMsExecute::GetApiClass() const
{
	return UBeamVenlyProviderServiceMsApi::StaticClass();
}

UClass* UK2BeamNode_ApiRequest_VenlyProviderServiceMsExecute::GetRequestClass() const
{
	return UVenlyProviderServiceMsExecuteRequest::StaticClass();
}

UClass* UK2BeamNode_ApiRequest_VenlyProviderServiceMsExecute::GetResponseClass() const
{
	return UVenlyProviderServiceMsExecuteResponse::StaticClass();
}

FString UK2BeamNode_ApiRequest_VenlyProviderServiceMsExecute::GetRequestSuccessDelegateName() const
{
	return TEXT("OnVenlyProviderServiceMsExecuteSuccess");
}

FString UK2BeamNode_ApiRequest_VenlyProviderServiceMsExecute::GetRequestErrorDelegateName() const
{
	return TEXT("OnVenlyProviderServiceMsExecuteError");
}

FString UK2BeamNode_ApiRequest_VenlyProviderServiceMsExecute::GetRequestCompleteDelegateName() const
{
	return TEXT("OnVenlyProviderServiceMsExecuteComplete");
}

#undef LOCTEXT_NAMESPACE
