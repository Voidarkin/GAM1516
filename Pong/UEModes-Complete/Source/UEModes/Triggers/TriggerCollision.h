// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerCollision.generated.h"

//WEEK 7
UENUM(BlueprintType)
enum class EType : uint8 {	
	LEFT = 0  UMETA(DisplayName = "LEFT"),
	RIGHT = 1 UMETA(DisplayName = "RIGHT"),
	XBOUND = 2 UMETA(DisplayName = "XBound"),
	ZBOUND = 3 UMETA(DisplayName = "ZBound")
};


UCLASS()
class UEMODES_API ATriggerCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerCollision();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom")
		class UBoxComponent* TriggerBox;

	//WEEK 7
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
		TEnumAsByte<EType> type;
	void SetType(EType newType);
	EType GetType() { return type; }

	//WEEK 7
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	AActor* MiddleOfField;
	void SetMiddleFieldActor(AActor* actor);
	AActor* GetMiddleFieldActor() { return MiddleOfField; }

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
