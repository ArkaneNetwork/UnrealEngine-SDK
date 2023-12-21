#pragma once

#include "CoreMinimal.h"
#include "BeamableCore/Public/AutoGen/RoleMapping.h"

#include "RoleMappingLibrary.generated.h"


UCLASS(BlueprintType, Category="Beam")
class BEAMABLECORE_API URoleMappingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="Beam|Json", DisplayName="Beam - RoleMapping To JSON String")
	static FString RoleMappingToJsonString(const URoleMapping* Serializable, const bool Pretty);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Make RoleMapping", meta=(DefaultToSelf="Outer", AdvancedDisplay="Outer", NativeMakeFunc))
	static URoleMapping* Make(FString ProjectId, FString Role, UObject* Outer);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Break RoleMapping", meta=(NativeBreakFunc))
	static void Break(const URoleMapping* Serializable, FString& ProjectId, FString& Role);
};