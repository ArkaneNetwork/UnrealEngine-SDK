
#include "BeamableCore/Public/AutoGen/SubSystems/Accounts/BasicAccountsGetMeRequest.h"

void UBasicAccountsGetMeRequest::BuildVerb(FString& VerbString) const
{
	VerbString = TEXT("GET");
}

void UBasicAccountsGetMeRequest::BuildRoute(FString& RouteString) const
{
	FString Route = TEXT("/basic/accounts/me");
	
	
	FString QueryParams = TEXT("");
	QueryParams.Reserve(1024);
	bool bIsFirstQueryParam = true;
	
	RouteString.Appendf(TEXT("%s%s"), *Route, *QueryParams);		
}

void UBasicAccountsGetMeRequest::BuildBody(FString& BodyString) const
{
	
}

UBasicAccountsGetMeRequest* UBasicAccountsGetMeRequest::Make(UObject* RequestOwner, TMap<FString, FString> CustomHeaders)
{
	UBasicAccountsGetMeRequest* Req = NewObject<UBasicAccountsGetMeRequest>(RequestOwner);
	Req->CustomHeaders = TMap{CustomHeaders};

	// Pass in Path and Query Parameters (Blank if no path parameters exist)
	
	
	// Makes a body and fill up with parameters (Blank if no body parameters exist)
	

	return Req;
}
