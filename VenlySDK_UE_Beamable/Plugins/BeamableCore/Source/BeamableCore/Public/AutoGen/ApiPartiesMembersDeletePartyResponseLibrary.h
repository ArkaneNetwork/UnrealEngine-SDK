#pragma once

#include "CoreMinimal.h"
#include "BeamableCore/Public/AutoGen/ApiPartiesMembersDeletePartyResponse.h"

#include "ApiPartiesMembersDeletePartyResponseLibrary.generated.h"


UCLASS(BlueprintType, Category="Beam")
class BEAMABLECORE_API UApiPartiesMembersDeletePartyResponseLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="Beam|Json", DisplayName="Beam - ApiPartiesMembersDeletePartyResponse To JSON String")
	static FString ApiPartiesMembersDeletePartyResponseToJsonString(const UApiPartiesMembersDeletePartyResponse* Serializable, const bool Pretty);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Make ApiPartiesMembersDeletePartyResponse", meta=(DefaultToSelf="Outer", AdvancedDisplay="Outer", NativeMakeFunc))
	static UApiPartiesMembersDeletePartyResponse* Make(UObject* Outer);

	
};