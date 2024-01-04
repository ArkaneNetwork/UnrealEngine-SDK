// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BindBeamableMicroservice.h"

#include "Utils/VyUtils.h"
#include "VyBeamableProxy.h"
#include "AutoGen/SubSystems/BeamVenlyProviderServiceMsApi.h"

// Sets default values
ACPP_BindBeamableMicroservice::ACPP_BindBeamableMicroservice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_BindBeamableMicroservice::BeginPlay()
{
	Super::BeginPlay();

	//Bind the Microservice to the Venly SDK (execute for each call made with any of the Venly APIs)
	UVyBeamableProxy::Get()->OnExecuteRequest.BindLambda([this](const FString& Request, const FVyOnExecuteProviderRequestComplete& OnComplete) {

		//Initialize the Microservice Request Data
		auto microserviceRequest = UVenlyProviderServiceMsExecuteRequest::Make(Request, this, {});
		auto requestContext = FBeamRequestContext{};
		auto handler = FOnVenlyProviderServiceMsExecuteFullResponse{};

		//Handle the Microservice Response
		handler.BindLambda([OnComplete](FVenlyProviderServiceMsExecuteFullResponse response) {

			//Notify Venly that a Response is received
			FVyBeamableMicroserviceResponse MicroserviceResponse{};

			//Set the Response 'Success' property
			MicroserviceResponse.Success = response.State == EBeamFullResponseState::RS_Success;

			//Set the Response 'Result' or 'Error' property
			if (MicroserviceResponse.Success) MicroserviceResponse.Result = response.SuccessData->Value;
			else MicroserviceResponse.Error = response.ErrorData;

			//Invoke OnComplete
			OnComplete.ExecuteIfBound(MicroserviceResponse);
		});
		
		//Call the Microservice (Execute)
		UBeamVenlyProviderServiceMsApi::GetSelf()->CPP_Execute(FUserSlot{ TEXT("Player0") }, microserviceRequest, handler, requestContext, {}, this);
	});
}

// Called every frame
void ACPP_BindBeamableMicroservice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

