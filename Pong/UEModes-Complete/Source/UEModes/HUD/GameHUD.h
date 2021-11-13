// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class UEMODES_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
    AGameHUD();

    UPROPERTY(EditAnywhere, Category = "Game Hud")
        TSubclassOf<class UUserWidget> PlayerGuiClass;

    UPROPERTY()
        class UUserWidget* PlayerGui;
	
protected:
    virtual void BeginPlay() override;
};
