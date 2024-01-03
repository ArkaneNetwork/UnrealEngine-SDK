
#include "VenlySDK_UE_Beamable\AutoGen/VenlyProviderServiceExecuteRequestArgsLibrary.h"

#include "CoreMinimal.h"


FString UVenlyProviderServiceExecuteRequestArgsLibrary::VenlyProviderServiceExecuteRequestArgsToJsonString(const UVenlyProviderServiceExecuteRequestArgs* Serializable, const bool Pretty)
{
	FString Result = FString{};
	if(Pretty)
	{
		TUnrealPrettyJsonSerializer JsonSerializer = TJsonStringWriter<TPrettyJsonPrintPolicy<wchar_t>>::Create(&Result);
		Serializable->BeamSerialize(JsonSerializer);
		JsonSerializer->Close();
	}
	else
	{
		TUnrealJsonSerializer JsonSerializer = TJsonStringWriter<TCondensedJsonPrintPolicy<wchar_t>>::Create(&Result);
		Serializable->BeamSerialize(JsonSerializer);
		JsonSerializer->Close();			
	}
	return Result;
}	

UVenlyProviderServiceExecuteRequestArgs* UVenlyProviderServiceExecuteRequestArgsLibrary::Make(FString Request, UObject* Outer)
{
	auto Serializable = NewObject<UVenlyProviderServiceExecuteRequestArgs>(Outer);
	Serializable->Request = Request;
	
	return Serializable;
}

void UVenlyProviderServiceExecuteRequestArgsLibrary::Break(const UVenlyProviderServiceExecuteRequestArgs* Serializable, FString& Request)
{
	Request = Serializable->Request;
		
}

