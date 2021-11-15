// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "UEModes/Triggers/TriggerCollision.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionSphere = CreateDefaultSubobject<USphereComponent>("SceneRoot");
	CollisionSphere->SetSphereRadius(75); //100,10,100
	CollisionSphere->SetSimulatePhysics(false);
	CollisionSphere->SetCollisionProfileName("BlockAll");
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionSphere->GetBodyInstance()->bLockXRotation = true;
	CollisionSphere->GetBodyInstance()->bLockZRotation = true;
	CollisionSphere->GetBodyInstance()->bLockYTranslation = true;
	CollisionSphere->SetNotifyRigidBodyCollision(true);
	CollisionSphere->OnComponentHit.AddDynamic(this, &ABall::OnHit);
	SetRootComponent(CollisionSphere);

	BallSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Paper Sprite Component"));
	BallSprite->SetupAttachment(RootComponent);
	BallSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Velocity = FVector(FMath::RandRange(-1, 1),0, FMath::RandRange(-1, 1));
	Speed = 500.0f;
	Timer = 2.0f;
	BoredomTimer = 18.0f;
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

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

