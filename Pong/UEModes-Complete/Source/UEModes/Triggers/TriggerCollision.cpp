// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerCollision.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "UEModes/Actors/Ball.h"
#include "UEModes/GameMode/UEGameMode.h"
#include "UEModes/GameState/UEGameState.h"
#include "UEModes/Pawn/CollidingPawn.h"
#include "UEModes/PlayerState/UEPlayerState.h"
#include "Math/UnrealMathUtility.h"
// Sets default values
ATriggerCollision::ATriggerCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("SceneRoot");
	TriggerBox->SetBoxExtent(FVector(120, 50, 140)); //100,10,100
	TriggerBox->SetSimulatePhysics(false);
	TriggerBox->SetCollisionProfileName("OverlapAllDynamic");
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerCollision::BeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerCollision::EndOverlap);
	
	SetRootComponent(TriggerBox);

}

void ATriggerCollision::SetType(EType newType)
{
	type = newType;
}

void ATriggerCollision::SetMiddleFieldActor(AActor* actor)
{
	MiddleOfField = actor;
}

void ATriggerCollision::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("Overlap Begin"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Overlapping Actor = %s"), *OtherActor->GetName()));
		}

		if(OtherActor->IsA<ABall>())
		{
			ABall* ball = Cast<ABall>(OtherActor);
			if (type == EType::XBOUND || type == EType::ZBOUND) {
				
				FVector CurrentPosition = ball->GetActorLocation();
				if (type == EType::XBOUND) {
					//Paddle or any Left and Right Boundaries
					ball->Velocity.X = -ball->Velocity.X;

					if (GetParentActor()->IsA<ACollidingPawn>()) { //change to paddle later possibly
						
					}

				}
				else if (type == EType::ZBOUND) {
					//Top and Bottom Boundaries
					ball->Velocity.Z = -ball->Velocity.Z;

				}
				ball->SetActorLocation(CurrentPosition + ball->Velocity);

			}
			else
			{
				ball->TeleportTo(MiddleOfField->GetActorLocation(), FRotator::ZeroRotator);
				//Make sure it isnt just bouncing up and down
				float RandomX;
				do {
					RandomX = FMath::RandRange(-1, 1);
				} while (RandomX <= 0.3f && RandomX >= -0.3f);
				ball->Velocity = FVector(RandomX, 0, FMath::RandRange(-1, 1));


				AUEGameMode* gameMode = Cast<AUEGameMode>(GetWorld()->GetAuthGameMode());
				AUEGameState* gameState = gameMode->GetGameState<AUEGameState>();
				AUEPlayerState* playerState = Cast<AUEPlayerState>(gameState->PlayerArray[0]);
				playerState->NumberOfGoals++;
				if (type == EType::LEFT)
					gameState->NumberOfLeftGoals++;
				else if(type == EType::RIGHT)
					gameState->NumberOfRightGoals++;


			}
		}
		else if (OtherActor->IsA<ACollidingPawn>())
		{
			// Vertical bounds 
			ACollidingPawn* pawn = Cast<ACollidingPawn>(OtherActor);
			
			FVector CurrentLocation = pawn->GetActorLocation();
			float BoxZPos = GetActorLocation().Z;
			//IF MovementForward NOT EQUAL to 0
			if (CurrentLocation.Z < BoxZPos) {
				CurrentLocation = pawn->GetActorLocation() + GetActorUpVector() * -5;
			}
			else if (CurrentLocation.Z > BoxZPos)
			{
				CurrentLocation = pawn->GetActorLocation() + GetActorUpVector() * 5;
			}
			
			pawn->SetActorLocation(CurrentLocation);


		}
	}
}

void ATriggerCollision::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

// Called when the game starts or when spawned
void ATriggerCollision::BeginPlay()
{
	Super::BeginPlay();
	DrawDebugBox(GetWorld(), GetActorLocation(), TriggerBox->GetUnscaledBoxExtent(), FColor::Green, true, 999, 0, 5);
}

// Called every frame
void ATriggerCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

