#pragma once

#include "CoreMinimal.h"
#include "VenlySDK_UE_Beamable\AutoGen/VenlyProviderServiceMsExecuteResponse.h"

#include "VenlyProviderServiceMsExecuteResponseLibrary.generated.h"


UCLASS(BlueprintType, Category="Beam")
class VENLYSDK_UE_BEAMABLE_API UVenlyProviderServiceMsExecuteResponseLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="Beam|Json", DisplayName="Beam - VenlyProviderServiceMsExecuteResponse To JSON String")
	static FString VenlyProviderServiceMsExecuteResponseToJsonString(const UVenlyProviderServiceMsExecuteResponse* Serializable, const bool Pretty);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Make VenlyProviderServiceMsExecuteResponse", meta=(DefaultToSelf="Outer", AdvancedDisplay="Outer", NativeMakeFunc))
	static UVenlyProviderServiceMsExecuteResponse* Make(FString Value, UObject* Outer);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Break VenlyProviderServiceMsExecuteResponse", meta=(NativeBreakFunc))
	static void Break(const UVenlyProviderServiceMsExecuteResponse* Serializable, FString& Value);
};