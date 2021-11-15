// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class UEMODES_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	ABall();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom")
		class USphereComponent* CollisionSphere;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Actor Visual")
		class UPaperSpriteComponent* BallSprite;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	class UPrimitiveComponent* GetPhysicsComponent();
	
	FVector Velocity;
	FVector GetVelocity() { return Velocity; }
	void SetVelocity(FVector newVelocity);

	float Speed;
	void SetTimer(float value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Goal)
		AActor* MiddleOfField;

	void SetBoredomTimer(float value);

private:

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	float Timer;
	float BoredomTimer;
};
