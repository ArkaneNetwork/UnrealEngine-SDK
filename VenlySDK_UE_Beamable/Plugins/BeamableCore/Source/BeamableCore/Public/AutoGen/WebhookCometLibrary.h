#pragma once

#include "CoreMinimal.h"
#include "BeamableCore/Public/AutoGen/WebhookComet.h"

#include "WebhookCometLibrary.generated.h"


UCLASS(BlueprintType, Category="Beam")
class BEAMABLECORE_API UWebhookCometLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="Beam|Json", DisplayName="Beam - WebhookComet To JSON String")
	static FString WebhookCometToJsonString(const UWebhookComet* Serializable, const bool Pretty);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Make WebhookComet", meta=(DefaultToSelf="Outer", AdvancedDisplay="Description, Outer", NativeMakeFunc))
	static UWebhookComet* Make(FString Method, URouteVariables* Variables, FString Symbol, UServiceRoute* Route, URouteParameters* Parameters, FOptionalString Description, UObject* Outer);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Break WebhookComet", meta=(NativeBreakFunc))
	static void Break(const UWebhookComet* Serializable, FString& Method, URouteVariables*& Variables, FString& Symbol, UServiceRoute*& Route, URouteParameters*& Parameters, FOptionalString& Description);
};