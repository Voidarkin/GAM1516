// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "UEPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UEMODES_API AUEPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AUEPlayerState();

	UFUNCTION(BlueprintCallable, Category = "Goals")
		int GetNumGoals();

	int NumberOfGoals;
};
