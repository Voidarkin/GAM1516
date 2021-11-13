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

// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
	SpringArm->bDoCollisionTest = false; //Disable Spring Arm Collision

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
	/*
	SpawnPointComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	SpawnPointComponent->SetupAttachment(RootComponent);
	SpawnPointComponent->ArrowSize = 5.f;
	SpawnPointComponent->SetRelativeRotation(FRotator(50.f, 0.f, 0.f));
	SpawnPointComponent->SetRelativeLocation(FVector(0.f, 100.f, 0.f));
	*/
	//Setup collision boxes
	/*
	LeftGoal = CreateDefaultSubobject<ATriggerCollision>("LeftGoal");
	LeftGoal->SetType(EType::LEFT);
	LeftGoal->SetMiddleFieldActor(MiddleOfField);

	RightGoal = CreateDefaultSubobject<ATriggerCollision>("RightGoal");
	RightGoal->SetType(EType::RIGHT);
	RightGoal->SetMiddleFieldActor(MiddleOfField);

	TopBoundary = CreateDefaultSubobject<ATriggerCollision>("TopBoundary");
	TopBoundary->SetType(EType::ZBOUND);
	TopBoundary->SetMiddleFieldActor(MiddleOfField);

	BottomBoundary = CreateDefaultSubobject<ATriggerCollision>("BottomBoundary");
	BottomBoundary->SetType(EType::ZBOUND);
	BottomBoundary->SetMiddleFieldActor(MiddleOfField);
	*/



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

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
	
	//FTimerManager& Timer = GetWorldTimerManager();
	//Timer.SetTimer(SpawnTimer, this, &AActorSpawner::SpawnActor, SpawnDelay, true);
}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
void ABoard::SpawnActor()
{
	//IF the BallTemplate NOT EQUAL to nullptr
	if (BallTemplate != nullptr)
	{
		//DECLARE a variable called World of type const UWorld* and assign it to the return value of GetWorld()
		UWorld* World = GetWorld();
		//IF World is NOT EQUAL to nullptr
		if (World)
		{
			//DECLARE a variable called SpawnParams of type FActorSpawnParameters
			FActorSpawnParameters SpawnParams;
			//SET SpawnParams Owner to this
			SpawnParams.Owner = this;
			//SET SpawnParams SpawnCollisionHandlingOverride property to ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			//DECLARE a variable called SpawnTransform of type FTransform and assign it to the return value of SpawnPointComponent->GetComponentTransform()
			FTransform SpawnTransform = SpawnPointComponent->GetComponentTransform();
			//DECLARE a variable called SpawnedActor of type AFallingActor* and assign it to the return value of World->SpawnActor<type>(args..), 
		//passing in AFallingActor as the type argument (inside <>) and in function arguments pass --> BallTemplate, SpawnTransform, SpawnParams
			pBall = World->SpawnActor<ABall>(BallTemplate, SpawnTransform, SpawnParams);
			//IF SpawnedActor NOT EQUAL to nullptr
			if (pBall)
			{

				//DECLARE a variable called direction of type FVector and assign it to the return value of --> FRotationMatrix(SpawnTransform.Rotator()).GetScaledAxis(EAxis::X)
						//FVector direction = FRotationMatrix(SpawnTransform.Rotator()).GetScaledAxis(EAxis::X);
				//DECLARE a variable called physicsComponent of type UPrimitiveComponent* and assign it to the return value of --> SpawnedActor->GetPhysicsComponent()
				//UPrimitiveComponent* physicsComponent = SpawnedActor->GetPhysicsComponent();
				//CALL AddForce(..) on the physicsComponent and pass in --> direction * 1000000, NAME_None, true
				//physicsComponent->AddForce(direction * 10000, NAME_None, true);
			}
			//ENDIF SpawnedActor NOT EQUAL to nullptr
		}
		//ENDIF World is NOT EQUAL to nullptr

	}//ENDIF the BallTemplate NOT EQUAL to nullptr

}
*/