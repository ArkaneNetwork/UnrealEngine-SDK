
#pragma once

#include "CoreMinimal.h"
#include "BeamBackend/BeamBaseRequestInterface.h"
#include "BeamBackend/BeamRequestContext.h"
#include "BeamBackend/BeamErrorResponse.h"
#include "BeamBackend/BeamFullResponse.h"

#include "VenlySDK_UE_Beamable\AutoGen/VenlyProviderServiceExecuteRequestArgs.h"
#include "VenlySDK_UE_Beamable\AutoGen/VenlyProviderServiceMsExecuteResponse.h"

#include "VenlyProviderServiceMsExecuteRequest.generated.h"

UCLASS(BlueprintType)
class VENLYSDK_UE_BEAMABLE_API UVenlyProviderServiceMsExecuteRequest : public UObject, public IBeamBaseRequestInterface
{
	GENERATED_BODY()
	
public:

	// Path Params
	
	
	// Query Params
	

	// Body Params
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="", Category="Beam")
	UVenlyProviderServiceExecuteRequestArgs* Body = {};

	// Beam Base Request Declaration
	UVenlyProviderServiceMsExecuteRequest() = default;

	virtual void BuildVerb(FString& VerbString) const override;
	virtual void BuildRoute(FString& RouteString) const override;
	virtual void BuildBody(FString& BodyString) const override;

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category="Beam|Backend|VenlyProviderServiceMs", DisplayName="Beam - Make VenlyProviderServiceMsExecute",  meta=(DefaultToSelf="RequestOwner", AdvancedDisplay="RequestOwner", AutoCreateRefTerm="CustomHeaders"))
	static UVenlyProviderServiceMsExecuteRequest* Make(FString _Request, UObject* RequestOwner, TMap<FString, FString> CustomHeaders);
};

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnVenlyProviderServiceMsExecuteSuccess, FBeamRequestContext, Context, UVenlyProviderServiceMsExecuteRequest*, Request, UVenlyProviderServiceMsExecuteResponse*, Response);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnVenlyProviderServiceMsExecuteError, FBeamRequestContext, Context, UVenlyProviderServiceMsExecuteRequest*, Request, FBeamErrorResponse, Error);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnVenlyProviderServiceMsExecuteComplete, FBeamRequestContext, Context, UVenlyProviderServiceMsExecuteRequest*, Request);

using FVenlyProviderServiceMsExecuteFullResponse = FBeamFullResponse<UVenlyProviderServiceMsExecuteRequest*, UVenlyProviderServiceMsExecuteResponse*>;
DECLARE_DELEGATE_OneParam(FOnVenlyProviderServiceMsExecuteFullResponse, FVenlyProviderServiceMsExecuteFullResponse);
