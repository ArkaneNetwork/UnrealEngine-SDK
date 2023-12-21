

#include "BeamableCoreBlueprintNodes/Public/BeamFlow/ApiRequest/AutoGen/Social/K2BeamNode_ApiRequest_PostFriendsMake.h"

#include "BeamK2.h"

#include "BeamableCore/Public/AutoGen/SubSystems/BeamSocialApi.h"
#include "BeamableCore/Public/AutoGen/SubSystems/Social/PostFriendsMakeRequest.h"
#include "BeamableCore/Public/AutoGen/CommonResponse.h"

#define LOCTEXT_NAMESPACE "K2BeamNode_ApiRequest_PostFriendsMake"

using namespace BeamK2;

FName UK2BeamNode_ApiRequest_PostFriendsMake::GetSelfFunctionName() const
{
	return GET_FUNCTION_NAME_CHECKED(UBeamSocialApi, GetSelf);
}

FName UK2BeamNode_ApiRequest_PostFriendsMake::GetRequestFunctionName() const
{
	return GET_FUNCTION_NAME_CHECKED(UBeamSocialApi, PostFriendsMake);
}

FName UK2BeamNode_ApiRequest_PostFriendsMake::GetMakeFunctionName() const
{
	return GET_FUNCTION_NAME_CHECKED(UPostFriendsMakeRequest, Make);
}

FString UK2BeamNode_ApiRequest_PostFriendsMake::GetServiceName() const
{
	return TEXT("Social");
}

FString UK2BeamNode_ApiRequest_PostFriendsMake::GetEndpointName() const
{
	return TEXT("PostFriendsMake");
}

UClass* UK2BeamNode_ApiRequest_PostFriendsMake::GetApiClass() const
{
	return UBeamSocialApi::StaticClass();
}

UClass* UK2BeamNode_ApiRequest_PostFriendsMake::GetRequestClass() const
{
	return UPostFriendsMakeRequest::StaticClass();
}

UClass* UK2BeamNode_ApiRequest_PostFriendsMake::GetResponseClass() const
{
	return UCommonResponse::StaticClass();
}

FString UK2BeamNode_ApiRequest_PostFriendsMake::GetRequestSuccessDelegateName() const
{
	return TEXT("OnPostFriendsMakeSuccess");
}

FString UK2BeamNode_ApiRequest_PostFriendsMake::GetRequestErrorDelegateName() const
{
	return TEXT("OnPostFriendsMakeError");
}

FString UK2BeamNode_ApiRequest_PostFriendsMake::GetRequestCompleteDelegateName() const
{
	return TEXT("OnPostFriendsMakeComplete");
}

#undef LOCTEXT_NAMESPACE
