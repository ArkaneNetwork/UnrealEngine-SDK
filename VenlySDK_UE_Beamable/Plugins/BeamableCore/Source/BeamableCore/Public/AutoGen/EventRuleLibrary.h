#pragma once

#include "CoreMinimal.h"
#include "BeamableCore/Public/AutoGen/EventRule.h"

#include "EventRuleLibrary.generated.h"


UCLASS(BlueprintType, Category="Beam")
class BEAMABLECORE_API UEventRuleLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="Beam|Json", DisplayName="Beam - EventRule To JSON String")
	static FString EventRuleToJsonString(const UEventRule* Serializable, const bool Pretty);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Make EventRule", meta=(DefaultToSelf="Outer", AdvancedDisplay="Outer", NativeMakeFunc))
	static UEventRule* Make(FString Rule, FString Value, UObject* Outer);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Break EventRule", meta=(NativeBreakFunc))
	static void Break(const UEventRule* Serializable, FString& Rule, FString& Value);
};