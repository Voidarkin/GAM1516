// Fill out your copyright notice in the Description page of Project Settings.

#include "PaddleAI.h"
#include "CollidingPawn.h"

#include "PaperSpriteComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

#include "GameFramework/SpringArmComponent.h"

#include "UEModes/Actors/Ball.h"

APaddleAI::APaddleAI() : Super()
{
}

void APaddleAI::Tick(float DeltaTime)
{
	MovementVertical = 0;
	FVector NewLocation = GetActorLocation();
	if (pBall)
	{
		if (pBall->GetActorLocation().Z > NewLocation.Z) //if ball is above, move up
		{
			MovementVertical = 1;
		}
		else if (pBall->GetActorLocation().Z < NewLocation.Z) //if ball is below, move down
		{
			MovementVertical = -1;
		}
		if (MovementVertical != 0)
		{
			NewLocation = GetActorLocation() + (GetActorUpVector() * MovementVertical * Speed * DeltaTime);
		}
	}
	SetActorLocation(NewLocation);
}

void APaddleAI::BeginPlay()
{
	Super::BeginPlay();
}