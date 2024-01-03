
#include "VenlySDK_UE_Beamable\AutoGen/VenlyProviderServiceExecuteRequestArgs.h"





void UVenlyProviderServiceExecuteRequestArgs::BeamSerializeProperties(TUnrealJsonSerializer& Serializer) const
{
	Serializer->WriteValue(TEXT("request"), Request);
}

void UVenlyProviderServiceExecuteRequestArgs::BeamSerializeProperties(TUnrealPrettyJsonSerializer& Serializer) const
{
	Serializer->WriteValue(TEXT("request"), Request);		
}

void UVenlyProviderServiceExecuteRequestArgs::BeamDeserializeProperties(const TSharedPtr<FJsonObject>& Bag)
{
	Request = Bag->GetStringField(TEXT("request"));
}



