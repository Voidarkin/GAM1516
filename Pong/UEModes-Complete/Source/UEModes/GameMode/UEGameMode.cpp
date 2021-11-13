// Fill out your copyright notice in the Description page of Project Settings.


#include "UEGameMode.h"
#include "EngineUtils.h" //Needed for TActorIterator
#include "Kismet/GameplayStatics.h"
void AUEGameMode::BeginPlay()
{
    Super::BeginPlay();
    //FOR TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr
    for(TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
    	//CALL AddOnScreenDebugMessage() on GEngine passing in: -1, 15.0f, FColor::Green, "Actor: " + ActorItr->GetName()
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
            "Actor: " + ActorItr->GetName());
    }
    //ENDFOR

   //DECLARE a FString variable called command and assign it to "show collision"
    FString command = "show collision";
   //DECLARE a variable called controller of type APlayerController* and assign it to the return value of: UGameplayStatics::GetPlayerController(GetWorld(), 0)
    APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    
    if (controller) //Uncomment
    {
    	//CALL ConsoleCommand() on controller passing in command, false
        controller->ConsoleCommand(command, false);
	    //SET bShowMouseCursor on controller to true
        controller->bShowMouseCursor = true;
        //SET bEnableClickEvents on controller to true
        controller->bEnableClickEvents = true;
        //SET bEnableMouseOverEvents on controller to true
        controller->bEnableMouseOverEvents = true;

    }
}
