#pragma once

#include "CoreMinimal.h"
#include "BeamableCore/Public/AutoGen/AccountRolesReport.h"

#include "AccountRolesReportLibrary.generated.h"


UCLASS(BlueprintType, Category="Beam")
class BEAMABLECORE_API UAccountRolesReportLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="Beam|Json", DisplayName="Beam - AccountRolesReport To JSON String")
	static FString AccountRolesReportToJsonString(const UAccountRolesReport* Serializable, const bool Pretty);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Make AccountRolesReport", meta=(DefaultToSelf="Outer", AdvancedDisplay="Outer", NativeMakeFunc))
	static UAccountRolesReport* Make(int64 AccountId, FString Email, TArray<URealmRolesReport*> Realms, UObject* Outer);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Break AccountRolesReport", meta=(NativeBreakFunc))
	static void Break(const UAccountRolesReport* Serializable, int64& AccountId, FString& Email, TArray<URealmRolesReport*>& Realms);
};