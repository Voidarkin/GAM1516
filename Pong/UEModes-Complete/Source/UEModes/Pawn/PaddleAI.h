// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollidingPawn.h"
#include "PaddleAI.generated.h"


/**
 * 
 */
UCLASS()
class UEMODES_API APaddleAI : public ACollidingPawn
{
	GENERATED_BODY()

public:

	APaddleAI();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
