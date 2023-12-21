
#include "BeamableCore/Public/AutoGen/SubSystems/Content/PostContentRequest.h"

void UPostContentRequest::BuildVerb(FString& VerbString) const
{
	VerbString = TEXT("POST");
}

void UPostContentRequest::BuildRoute(FString& RouteString) const
{
	FString Route = TEXT("/basic/content/");
	
	
	FString QueryParams = TEXT("");
	QueryParams.Reserve(1024);
	bool bIsFirstQueryParam = true;
	
	RouteString.Appendf(TEXT("%s%s"), *Route, *QueryParams);		
}

void UPostContentRequest::BuildBody(FString& BodyString) const
{
	ensureAlways(Body);

	TUnrealJsonSerializer JsonSerializer = TJsonStringWriter<TCondensedJsonPrintPolicy<wchar_t>>::Create(&BodyString);
	Body->BeamSerialize(JsonSerializer);
	JsonSerializer->Close();
}

UPostContentRequest* UPostContentRequest::Make(TArray<UContentDefinition*> _Content, UObject* RequestOwner, TMap<FString, FString> CustomHeaders)
{
	UPostContentRequest* Req = NewObject<UPostContentRequest>(RequestOwner);
	Req->CustomHeaders = TMap{CustomHeaders};

	// Pass in Path and Query Parameters (Blank if no path parameters exist)
	
	
	// Makes a body and fill up with parameters (Blank if no body parameters exist)
	Req->Body = NewObject<USaveContentRequestBody>(Req);
	Req->Body->Content = _Content;
	

	return Req;
}
