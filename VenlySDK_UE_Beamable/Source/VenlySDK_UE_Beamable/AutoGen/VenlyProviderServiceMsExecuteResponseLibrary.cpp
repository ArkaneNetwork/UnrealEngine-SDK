
#include "VenlySDK_UE_Beamable\AutoGen/VenlyProviderServiceMsExecuteResponseLibrary.h"

#include "CoreMinimal.h"


FString UVenlyProviderServiceMsExecuteResponseLibrary::VenlyProviderServiceMsExecuteResponseToJsonString(const UVenlyProviderServiceMsExecuteResponse* Serializable, const bool Pretty)
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

UVenlyProviderServiceMsExecuteResponse* UVenlyProviderServiceMsExecuteResponseLibrary::Make(FString Value, UObject* Outer)
{
	auto Serializable = NewObject<UVenlyProviderServiceMsExecuteResponse>(Outer);
	Serializable->Value = Value;
	
	return Serializable;
}

void UVenlyProviderServiceMsExecuteResponseLibrary::Break(const UVenlyProviderServiceMsExecuteResponse* Serializable, FString& Value)
{
	Value = Serializable->Value;
		
}

