// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "UEModes/Triggers/TriggerCollision.h"
// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//1
	CollisionSphere = CreateDefaultSubobject<USphereComponent>("SceneRoot");
	//2
	CollisionSphere->SetSphereRadius(75); //100,10,100
	//3
	CollisionSphere->SetSimulatePhysics(false);
	//4 - Add a Step and show camera .
	CollisionSphere->SetCollisionProfileName("BlockAll");
	//5	
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//6
	CollisionSphere->GetBodyInstance()->bLockXRotation = true;
	CollisionSphere->GetBodyInstance()->bLockZRotation = true;
	CollisionSphere->GetBodyInstance()->bLockYTranslation = true;

	//
	CollisionSphere->SetNotifyRigidBodyCollision(true);

	//7
	CollisionSphere->OnComponentHit.AddDynamic(this, &ABall::OnHit);
	//8
	SetRootComponent(CollisionSphere);

	/**  paper sprite component */
	BallSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Paper Sprite Component"));
	BallSprite->SetupAttachment(RootComponent);
	BallSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Velocity = FVector(FMath::RandRange(-1, 1),0, FMath::RandRange(-1, 1));
	Speed = 500.0f;
	Timer = 2.0f;
	BoredomTimer = 18.0f;
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//DrawDebugSphere(GetWorld(), GetActorLocation(), CollisionSphere->GetUnscaledSphereRadius(), 26, FColor::Green, true, -1, 0, 5);

	//Move ball 

	if (Timer > 0)
	{
		Timer -= DeltaTime;
	}
	else
	{
		if (BoredomTimer > 0) {
			BoredomTimer -= DeltaTime;
		}
		else {
			BoredomTimer = 18.0f;
			TeleportTo(MiddleOfField->GetActorLocation(), FRotator::ZeroRotator);
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
			Velocity = FVector(RandomX, 0, 0.5f);
			SetTimer(2.0f);
			Speed = 500.0f;
		}


		FVector NewLocation = GetActorLocation();

		Velocity = Velocity.GetSafeNormal();
	
		Speed += DeltaTime;

		NewLocation = GetActorLocation() + (Velocity * Speed * DeltaTime);

		SetActorLocation(NewLocation);
	}
}
void ABall::Destroyed()
{
	Super::Destroyed();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("BALL DESTROYED")));
}

UPrimitiveComponent* ABall::GetPhysicsComponent()
{
	return Cast<UPrimitiveComponent>(CollisionSphere);
}

void ABall::SetVelocity(FVector newVelocity)
{
	Velocity = newVelocity;
}

void ABall::SetTimer(float value)
{
	Timer = value;
}

void ABall::SetBoredomTimer(float value)
{
	BoredomTimer = value;
}

void ABall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		if (GEngine)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("%s has Hit"), *OtherActor->GetName()));
		}
	}
}

