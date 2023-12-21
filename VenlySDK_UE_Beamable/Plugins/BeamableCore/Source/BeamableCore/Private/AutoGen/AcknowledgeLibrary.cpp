
#include "BeamableCore/Public/AutoGen/AcknowledgeLibrary.h"

#include "CoreMinimal.h"


FString UAcknowledgeLibrary::AcknowledgeToJsonString(const UAcknowledge* Serializable, const bool Pretty)
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

UAcknowledge* UAcknowledgeLibrary::Make(UObject* Outer)
{
	auto Serializable = NewObject<UAcknowledge>(Outer);
	
	return Serializable;
}



