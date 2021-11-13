// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawn.h"
#include "PaperSpriteComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

#include "GameFramework/SpringArmComponent.h"

#include "UEModes/Actors/Ball.h"

// Sets default values
ACollidingPawn::ACollidingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#pragma region Hierarchy 2

	PlayerRootCollisionBox = CreateDefaultSubobject<UBoxComponent>("SceneRoot");
	PlayerRootCollisionBox->SetBoxExtent(FVector(120, 50, 140)); //100,10,100
	PlayerRootCollisionBox->SetSimulatePhysics(false);
	PlayerRootCollisionBox->SetCollisionProfileName("OverlapAllDynamic");
	PlayerRootCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PlayerRootCollisionBox->GetBodyInstance()->bLockRotation = true;// BodyInstance.bLockXRotation = true; SetConstraintMode(EDOFMode::XZPlane)
	PlayerRootCollisionBox->GetBodyInstance()->bLockXRotation = true;
	PlayerRootCollisionBox->GetBodyInstance()->bLockYRotation = true;
	PlayerRootCollisionBox->GetBodyInstance()->bLockZRotation = true;
	PlayerRootCollisionBox->GetBodyInstance()->bLockYTranslation = true;
	PlayerRootCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACollidingPawn::BeginOverlap);
	PlayerRootCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACollidingPawn::EndOverlap);
	//6
	SetRootComponent(PlayerRootCollisionBox);

	//MISC
	//PlayerRootCollisionBox->SetEnableGravity(false);
	
	//OR
	//RootComponent = PlayerRoot;
	PawnSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>("Pawn Sprite");
	PawnSpriteComponent->SetupAttachment(RootComponent);
	PawnSpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SpringArm->SocketOffset = FVector(0.0f, 500.0f, 0.0f);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 10.0f;
	SpringArm->bDoCollisionTest = false; //Disable Spring Arm Collision

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Pawns Camera");
	FollowCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	FollowCamera->SetOrthoWidth(2000.0f);
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

#pragma endregion Hierarchy 2	

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	/*AutoPossessPlayer = EAutoReceiveInput::Player0;*/


	NormalSpeed = 250;
	FastSpeed = 350;
	Speed = NormalSpeed;
}

// Called when the game starts or when spawned
void ACollidingPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACollidingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector NewLocation = GetActorLocation();
	//IF MovementForward NOT EQUAL to 0
	if (MovementVertical != 0)
	{
	    //ASSIGN NewLocation to the the sum of the Actors Current Location and the product of the Actors Forward Vector and the MovementForward value i.e GetActorLocation() + (GetActorForwardVector() * MovementForward)
	    NewLocation = GetActorLocation() + (GetActorUpVector() * MovementVertical * Speed * DeltaTime);
	}
	//ENDIF	

	//CALL SetActorLocation() passing in NewLocation
	SetActorLocation(NewLocation);

}

// Called to bind functionality to input
void ACollidingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//PlayerInputComponent->BindAxis("MoveForward", this, &ACollidingPawn::MoveForward);
	//PlayerInputComponent->BindAction("Speed", IE_Pressed, this, &ACollidingPawn::SpeedUp);
	//PlayerInputComponent->BindAction("Speed", IE_Released, this, &ACollidingPawn::SlowDown);
	
}


void ACollidingPawn::MoveVertical(float value)
{
	MovementVertical = value;
}

void ACollidingPawn::SpeedUp()
{
	Speed = FastSpeed;
}

void ACollidingPawn::SlowDown()
{
	Speed = NormalSpeed;
}

void ACollidingPawn::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
			
			ball->SetBoredomTimer(18.0f);

			FVector CurrentPosition = ball->GetActorLocation();

			float difference;
			if (GetActorLocation().Z < ball->GetActorLocation().Z)
			{
				//ball higher
				difference = -(GetActorLocation().Z - ball->GetActorLocation().Z);
				
			}
			else if (GetActorLocation().Z > ball->GetActorLocation().Z)
			{
				//ball lower

				difference = (GetActorLocation().Z - (ball->GetActorLocation().Z));
			}
			else
			{
				
			}
			ball->Velocity.X = -ball->Velocity.X;
			
			float height = GetActorScale().Z / 2;
			ball->Velocity.Z += (difference / height / 2);
			ball->Velocity.Z = FMath::Clamp(ball->Velocity.Z, -0.75f, 0.75f);
			ball->SetActorLocation(CurrentPosition + ball->Velocity);

			
		}
	}
}

void ACollidingPawn::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

