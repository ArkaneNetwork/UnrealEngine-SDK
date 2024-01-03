
#include "VenlySDK_UE_Beamable\AutoGen/VenlyProviderServiceMsExecuteResponse.h"
#include "Serialization/BeamJsonUtils.h"



void UVenlyProviderServiceMsExecuteResponse::DeserializeRequestResponse(UObject* RequestData, FString ResponseContent)
{
	OuterOwner = RequestData;
	UBeamJsonUtils::DeserializeRawPrimitive<FString>(ResponseContent, Value, OuterOwner);
}

void UVenlyProviderServiceMsExecuteResponse::BeamSerializeProperties(TUnrealJsonSerializer& Serializer) const
{
	Serializer->WriteValue(TEXT("Value"), Value);
}

void UVenlyProviderServiceMsExecuteResponse::BeamSerializeProperties(TUnrealPrettyJsonSerializer& Serializer) const
{
	Serializer->WriteValue(TEXT("Value"), Value);		
}

void UVenlyProviderServiceMsExecuteResponse::BeamDeserializeProperties(const TSharedPtr<FJsonObject>& Bag)
{
	Value = Bag->GetStringField(TEXT("Value"));
}



