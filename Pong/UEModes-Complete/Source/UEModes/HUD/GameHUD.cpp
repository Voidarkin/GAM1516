// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

AGameHUD::AGameHUD()
{
    //static ConstructorHelpers::FClassFinder<UUserWidget> playerHud(TEXT("/Game/Blueprints/UI/PlayerUI"));
    //if (playerHud.Succeeded())
    //    PlayerGuiClass = playerHud.Class;
}
void AGameHUD::BeginPlay()
{
    //CREATE the PlayerGui widget by assigning the return value of CreateWidget<UUserWidget>() passing in: GetGameInstance(), PlayerGuiClass
    PlayerGui = CreateWidget<UUserWidget>(GetGameInstance(), PlayerGuiClass);
    //CALL AddToViewport() on PlayerGui
    PlayerGui->AddToViewport();

}
