#pragma once

#include "CoreMinimal.h"
#include "BeamableCore/Public/AutoGen/SendMsg.h"

#include "SendMsgLibrary.generated.h"


UCLASS(BlueprintType, Category="Beam")
class BEAMABLECORE_API USendMsgLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="Beam|Json", DisplayName="Beam - SendMsg To JSON String")
	static FString SendMsgToJsonString(const USendMsg* Serializable, const bool Pretty);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Make SendMsg", meta=(DefaultToSelf="Outer", AdvancedDisplay="Notification, Data, Outer", NativeMakeFunc))
	static USendMsg* Make(TArray<int64> To, FOptionalSendNotification Notification, FOptionalMapOfString Data, UObject* Outer);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Break SendMsg", meta=(NativeBreakFunc))
	static void Break(const USendMsg* Serializable, TArray<int64>& To, FOptionalSendNotification& Notification, FOptionalMapOfString& Data);
};