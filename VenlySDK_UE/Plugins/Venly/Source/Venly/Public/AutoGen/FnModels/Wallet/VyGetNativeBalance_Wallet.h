#pragma once

#include "Models/VyCoreModels.h"
#include "AutoGen/Models/Wallet/VyNativeTokenDto.h"

#include "VyGetNativeBalance_Wallet.generated.h"


//RESPONSE [Balance]
USTRUCT(BlueprintType)
struct VENLY_API FVyGetNativeBalanceResponse : public FVyApiResponse
{
	GENERATED_BODY();
	UPROPERTY(BlueprintReadOnly, Category="Venly") FVyNativeTokenDto Result;
};

//ON COMPLETE DELEGATE
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_OneParam(FVyOnGetNativeBalanceCompleteBP, FVyGetNativeBalanceResponse, Result);
