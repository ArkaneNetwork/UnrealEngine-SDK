#pragma once

#include "CoreMinimal.h"
#include "BeamableCore/Public/AutoGen/InFlightFailureResponse.h"

#include "InFlightFailureResponseLibrary.generated.h"


UCLASS(BlueprintType, Category="Beam")
class BEAMABLECORE_API UInFlightFailureResponseLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="Beam|Json", DisplayName="Beam - InFlightFailureResponse To JSON String")
	static FString InFlightFailureResponseToJsonString(const UInFlightFailureResponse* Serializable, const bool Pretty);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Make InFlightFailureResponse", meta=(DefaultToSelf="Outer", AdvancedDisplay="Outer", NativeMakeFunc))
	static UInFlightFailureResponse* Make(TArray<UInFlightFailure*> Failures, UObject* Outer);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Break InFlightFailureResponse", meta=(NativeBreakFunc))
	static void Break(const UInFlightFailureResponse* Serializable, TArray<UInFlightFailure*>& Failures);
};