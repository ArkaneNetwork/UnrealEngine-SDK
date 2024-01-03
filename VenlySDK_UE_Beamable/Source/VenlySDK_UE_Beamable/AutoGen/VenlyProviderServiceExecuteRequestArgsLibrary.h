#pragma once

#include "CoreMinimal.h"
#include "VenlySDK_UE_Beamable\AutoGen/VenlyProviderServiceExecuteRequestArgs.h"

#include "VenlyProviderServiceExecuteRequestArgsLibrary.generated.h"


UCLASS(BlueprintType, Category="Beam")
class VENLYSDK_UE_BEAMABLE_API UVenlyProviderServiceExecuteRequestArgsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="Beam|Json", DisplayName="Beam - VenlyProviderServiceExecuteRequestArgs To JSON String")
	static FString VenlyProviderServiceExecuteRequestArgsToJsonString(const UVenlyProviderServiceExecuteRequestArgs* Serializable, const bool Pretty);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Make VenlyProviderServiceExecuteRequestArgs", meta=(DefaultToSelf="Outer", AdvancedDisplay="Outer", NativeMakeFunc))
	static UVenlyProviderServiceExecuteRequestArgs* Make(FString Request, UObject* Outer);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Break VenlyProviderServiceExecuteRequestArgs", meta=(NativeBreakFunc))
	static void Break(const UVenlyProviderServiceExecuteRequestArgs* Serializable, FString& Request);
};