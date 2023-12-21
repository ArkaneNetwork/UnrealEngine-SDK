
#pragma once

#include "CoreMinimal.h"
#include "BeamBackend/BeamBaseRequestInterface.h"
#include "BeamBackend/BeamRequestContext.h"
#include "BeamBackend/BeamErrorResponse.h"
#include "BeamBackend/BeamFullResponse.h"

#include "BeamableCore/Public/AutoGen/MessageRequestBody.h"
#include "BeamableCore/Public/AutoGen/ApiMailboxPublishPostMailboxResponse.h"

#include "PostPublishRequest.generated.h"

UCLASS(BlueprintType)
class BEAMABLECORE_API UPostPublishRequest : public UObject, public IBeamBaseRequestInterface
{
	GENERATED_BODY()
	
public:

	// Path Params
	
	
	// Query Params
	

	// Body Params
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="", Category="Beam")
	UMessageRequestBody* Body = {};

	// Beam Base Request Declaration
	UPostPublishRequest() = default;

	virtual void BuildVerb(FString& VerbString) const override;
	virtual void BuildRoute(FString& RouteString) const override;
	virtual void BuildBody(FString& BodyString) const override;

	UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category="Beam|Backend|Mailbox", DisplayName="Beam - Make PostPublish",  meta=(DefaultToSelf="RequestOwner", AdvancedDisplay="_Body,_Cid,_Pid,_PlayerId,RequestOwner", AutoCreateRefTerm="CustomHeaders"))
	static UPostPublishRequest* Make(FOptionalString _Body, FOptionalString _Cid, FOptionalString _Pid, FOptionalString _PlayerId, UObject* RequestOwner, TMap<FString, FString> CustomHeaders);
};

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnPostPublishSuccess, FBeamRequestContext, Context, UPostPublishRequest*, Request, UApiMailboxPublishPostMailboxResponse*, Response);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnPostPublishError, FBeamRequestContext, Context, UPostPublishRequest*, Request, FBeamErrorResponse, Error);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnPostPublishComplete, FBeamRequestContext, Context, UPostPublishRequest*, Request);

using FPostPublishFullResponse = FBeamFullResponse<UPostPublishRequest*, UApiMailboxPublishPostMailboxResponse*>;
DECLARE_DELEGATE_OneParam(FOnPostPublishFullResponse, FPostPublishFullResponse);
