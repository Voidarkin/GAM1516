// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UEGameState.generated.h"

/**
 * 
 */
UCLASS()
class UEMODES_API AUEGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AUEGameState();

	UFUNCTION(BlueprintCallable, Category = "Goals")
		int GetNumberOfLeftGoals();

	UFUNCTION(BlueprintCallable, Category = "Goals")
		int GetNumberOfRightGoals();
	
	int NumberOfLeftGoals;
	int NumberOfRightGoals;
};
