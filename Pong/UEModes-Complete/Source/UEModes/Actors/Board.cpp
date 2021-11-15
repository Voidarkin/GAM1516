// Fill out your copyright notice in the Description page of Project Settings.

#include "Board.h"

#include "PaperSpriteComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "UEModes/Triggers/TriggerCollision.h"
#include "UEModes/Pawn/CollidingPawn.h"
#include "UEModes/Actors/Ball.h"
#include "UEModes/Spawner/ActorSpawner.h"
#include "UEModes/PlayerState/UEPlayerState.h"
#include "UEModes/GameMode/UEGameMode.h"
#include "UEModes/GameState/UEGameState.h"
#include "UObject/ConstructorHelpers.h"

ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = true;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>("Board Sprite");
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(SpriteComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 300.0f);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 10.0f;
	SpringArm->bDoCollisionTest = false;

	BoardCamera = CreateDefaultSubobject<UCameraComponent>("BoardCamera");
	BoardCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	BoardCamera->SetOrthoWidth(5000.0f);
	BoardCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	BoardCamera->bUsePawnControlRotation = false;
	BoardCamera->Activate(true);

	LeftGoal = CreateDefaultSubobject<UBoxComponent>("LeftGoal");
	LeftGoal->SetBoxExtent(FVector(120, 50, 140)); //100,10,100
	LeftGoal->SetSimulatePhysics(false);
	LeftGoal->SetCollisionProfileName("OverlapAllDynamic");
	LeftGoal->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeftGoal->OnComponentBeginOverlap.AddDynamic(this, &ABoard::BeginOverlap);
	LeftGoal->OnComponentEndOverlap.AddDynamic(this, &ABoard::EndOverlap);

	RightGoal = CreateDefaultSubobject<UBoxComponent>("RightGoal");
	RightGoal->SetBoxExtent(FVector(120, 50, 140)); //100,10,100
	RightGoal->SetSimulatePhysics(false);
	RightGoal->SetCollisionProfileName("OverlapAllDynamic");
	RightGoal->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightGoal->OnComponentBeginOverlap.AddDynamic(this, &ABoard::BeginOverlap);
	RightGoal->OnComponentEndOverlap.AddDynamic(this, &ABoard::EndOverlap);

	TopBoundary = CreateDefaultSubobject<UBoxComponent>("TopBoundary");
	TopBoundary->SetBoxExtent(FVector(120, 50, 140)); //100,10,100
	TopBoundary->SetSimulatePhysics(false);
	TopBoundary->SetCollisionProfileName("OverlapAllDynamic");
	TopBoundary->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TopBoundary->OnComponentBeginOverlap.AddDynamic(this, &ABoard::BeginOverlap);
	TopBoundary->OnComponentEndOverlap.AddDynamic(this, &ABoard::EndOverlap);

	BottomBoundary = CreateDefaultSubobject<UBoxComponent>("BottomBoundary");
	BottomBoundary->SetBoxExtent(FVector(120, 50, 140)); //100,10,100
	BottomBoundary->SetSimulatePhysics(false);
	BottomBoundary->SetCollisionProfileName("OverlapAllDynamic");
	BottomBoundary->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BottomBoundary->OnComponentBeginOverlap.AddDynamic(this, &ABoard::BeginOverlap);
	BottomBoundary->OnComponentEndOverlap.AddDynamic(this, &ABoard::EndOverlap);

}

void ABoard::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("Overlap Begin"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Overlapping Actor = %s"), *OtherActor->GetName()));
		}

		if (OtherActor->IsA<ABall>())
		{
			ABall* ball = Cast<ABall>(OtherActor);
			//if (type == EType::XBOUND || type == EType::ZBOUND) {
			UBoxComponent* box = Cast<UBoxComponent>(OverlappedComponent);
			if(box->GetFName() == "TopBoundary" || box->GetFName() == "BottomBoundary") 
			{
				FVector CurrentPosition = ball->GetActorLocation();
				
				//Top and Bottom Boundaries
				ball->Velocity.Z = -ball->Velocity.Z;

				ball->SetActorLocation(CurrentPosition + ball->Velocity);

			}
			else
			{
				ball->TeleportTo(MiddleOfField->GetActorLocation(), FRotator::ZeroRotator);
				//Make sure it isnt just bouncing up and down
				float RandomX;
				RandomX = FMath::RandRange(-1, 1);
				if (RandomX < 0) {
					RandomX = -1;
				}
				else {
					RandomX = 1;
				}
				float RandomZ;
				do {
					RandomZ = FMath::RandRange(-1, 1);
				} while ((RandomZ >= 0.9f && RandomZ <= 0.3f) && (RandomZ >= -0.9f && RandomZ >= -0.3f));	
				ball->Velocity = FVector(RandomX, 0, 0.5f);
				ball->SetTimer(2.0f);
				ball->Speed = 500.0f;

				AUEGameMode* gameMode = Cast<AUEGameMode>(GetWorld()->GetAuthGameMode());
				AUEGameState* gameState = gameMode->GetGameState<AUEGameState>();
				AUEPlayerState* playerState = Cast<AUEPlayerState>(gameState->PlayerArray[0]);
				//playerState->NumberOfGoals++;
				if (box->GetFName() == "LeftGoal")
					gameState->NumberOfRightGoals++;
				else if (box->GetFName() == "RightGoal")
					gameState->NumberOfLeftGoals++;
				ball->SetBoredomTimer(18.0f);

			}
		}
		else if (OtherActor->IsA<ACollidingPawn>())
		{
			// Vertical bounds 
			ACollidingPawn* pawn = Cast<ACollidingPawn>(OtherActor);
			UBoxComponent* box = Cast<UBoxComponent>(OverlappedComponent);
			FVector CurrentLocation = pawn->GetActorLocation();
			//IF MovementForward NOT EQUAL to 0
			if (box->GetFName() == "TopBoundary") {
				CurrentLocation = pawn->GetActorLocation() + GetActorUpVector() * -5;
			}
			else if (box->GetFName() == "BottomBoundary")
			{
				CurrentLocation = pawn->GetActorLocation() + GetActorUpVector() * 5;
			}

			pawn->SetActorLocation(CurrentLocation);


		}
	}
}

void ABoard::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("Overlap Ended"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("%s has left the Trigger Volume"), *OtherActor->GetName()));
		}
	}
}

void ABoard::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
