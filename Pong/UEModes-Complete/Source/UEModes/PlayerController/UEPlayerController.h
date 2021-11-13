// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UEPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UEMODES_API AUEPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

    virtual void OnPossess(APawn* aPawn) override;
    virtual void OnUnPossess() override;

protected:
    virtual void SetupInputComponent() override;

    virtual void AcknowledgePossession(APawn* PossesedPawn) override;

    void MoveVertical(float value);
	void SpeedUp();
	void SlowDown();

    class AUEPlayerState* MyPlayerState;
    class ACollidingPawn* MyPawn;
};
