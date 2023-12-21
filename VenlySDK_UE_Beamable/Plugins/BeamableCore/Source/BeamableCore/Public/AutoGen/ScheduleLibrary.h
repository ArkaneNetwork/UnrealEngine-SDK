#pragma once

#include "CoreMinimal.h"
#include "BeamableCore/Public/AutoGen/Schedule.h"

#include "ScheduleLibrary.generated.h"


UCLASS(BlueprintType, Category="Beam")
class BEAMABLECORE_API UScheduleLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="Beam|Json", DisplayName="Beam - Schedule To JSON String")
	static FString ScheduleToJsonString(const USchedule* Serializable, const bool Pretty);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Make Schedule", meta=(DefaultToSelf="Outer", AdvancedDisplay="Description, ActiveTo, Crons, Definitions, Outer", NativeMakeFunc))
	static USchedule* Make(FString ActiveFrom, FOptionalString Description, FOptionalString ActiveTo, FOptionalArrayOfString Crons, FOptionalArrayOfScheduleDefinition Definitions, UObject* Outer);

	UFUNCTION(BlueprintPure, Category="Beam|Backend", DisplayName="Beam - Break Schedule", meta=(NativeBreakFunc))
	static void Break(const USchedule* Serializable, FString& ActiveFrom, FOptionalString& Description, FOptionalString& ActiveTo, FOptionalArrayOfString& Crons, FOptionalArrayOfScheduleDefinition& Definitions);
};