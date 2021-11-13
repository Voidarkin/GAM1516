// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Board.generated.h"

UCLASS()
class UEMODES_API ABoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoard();

	UPROPERTY(EditAnywhere, Category = "Pawn Visual")
		class UPaperSpriteComponent* SpriteComponent;

	UPROPERTY(VisibleAnywhere, Category = "Board Camera")
		class UCameraComponent* BoardCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom")
		class USpringArmComponent* SpringArm;

	//testing
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision Box")
		class UBoxComponent* LeftGoal;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision Box")
		class UBoxComponent* RightGoal;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision Box")
		class UBoxComponent* TopBoundary;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision Box")
		class UBoxComponent* BottomBoundary;

	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision Box")
		class ATriggerCollision* LeftGoal;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision Box")
		class ATriggerCollision* RightGoal;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision Box")
		class ATriggerCollision* TopBoundary;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision Box")
		class ATriggerCollision* BottomBoundary;
*/


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
		class ABall* pBall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Goal)
		AActor* MiddleOfField;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
		//TSubclassOf<class ABall> BallTemplate;

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//TMap<FString, UBoxComponent*> name;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/* arrow component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* SpawnPointComponent;

	/** spawn timer  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
		FTimerHandle SpawnTimer;

	/** timer interval to spawn new
		falling actor after previous
		actor was destroyed  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
		float SpawnDelay = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
		int32 SpawnCount = 0;

	void SpawnActor();
};
