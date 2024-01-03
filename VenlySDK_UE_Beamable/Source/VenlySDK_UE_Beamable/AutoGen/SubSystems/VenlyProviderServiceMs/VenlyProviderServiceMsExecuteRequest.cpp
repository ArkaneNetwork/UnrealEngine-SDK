
#include "VenlySDK_UE_Beamable\AutoGen/SubSystems/VenlyProviderServiceMs/VenlyProviderServiceMsExecuteRequest.h"

void UVenlyProviderServiceMsExecuteRequest::BuildVerb(FString& VerbString) const
{
	VerbString = TEXT("POST");
}

void UVenlyProviderServiceMsExecuteRequest::BuildRoute(FString& RouteString) const
{
	FString Route = TEXT("micro_VenlyProviderService/Execute");
	
	
	FString QueryParams = TEXT("");
	QueryParams.Reserve(1024);
	bool bIsFirstQueryParam = true;
	
	RouteString.Appendf(TEXT("%s%s"), *Route, *QueryParams);		
}

void UVenlyProviderServiceMsExecuteRequest::BuildBody(FString& BodyString) const
{
	ensureAlways(Body);

	TUnrealJsonSerializer JsonSerializer = TJsonStringWriter<TCondensedJsonPrintPolicy<wchar_t>>::Create(&BodyString);
	Body->BeamSerialize(JsonSerializer);
	JsonSerializer->Close();
}

UVenlyProviderServiceMsExecuteRequest* UVenlyProviderServiceMsExecuteRequest::Make(FString _Request, UObject* RequestOwner, TMap<FString, FString> CustomHeaders)
{
	UVenlyProviderServiceMsExecuteRequest* Req = NewObject<UVenlyProviderServiceMsExecuteRequest>(RequestOwner);
	Req->CustomHeaders = TMap{CustomHeaders};

	// Pass in Path and Query Parameters (Blank if no path parameters exist)
	
	
	// Makes a body and fill up with parameters (Blank if no body parameters exist)
	Req->Body = NewObject<UVenlyProviderServiceExecuteRequestArgs>(Req);
	Req->Body->Request = _Request;
	

	return Req;
}
