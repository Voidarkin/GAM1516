// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorSpawner.h"
#include "Components/ArrowComponent.h"
#include "UEModes/Actors/Ball.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AActorSpawner::AActorSpawner()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    /* arrow component  */
    //CREATE the UArrowComponent by Calling CreateDefaultSubobject<>() and assign the Return of that function to SpawnPointComponent
    SpawnPointComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
    //ATTACH the SpawnPointComponent to the RootComponent by CALLING SetupAttachment(..) on SpawnPointComponent and pass in RootComponent 
    SpawnPointComponent->SetupAttachment(RootComponent);
    //SET the SpawnPointComponent's ArrowSize property to 5.0f
    SpawnPointComponent->ArrowSize = 5.f;
    //CALL SetRelativeRotation(..) on the SpawnPointComponent and pass in FRotator(50.f, 0.f, 0.f)
    SpawnPointComponent->SetRelativeRotation(FRotator(50.f, 0.f, 0.f));
    //CALL SetRelativeLocation(..) on the SpawnPointComponent and pass in FVector(0.f, 100.f, 0.f)
    SpawnPointComponent->SetRelativeLocation(FVector(0.f, 100.f, 0.f));

}

// Called when the game starts or when spawned
void AActorSpawner::BeginPlay()
{
    //CALL Super::BeginPlay()
    Super::BeginPlay();
    //DECLARE a variable called Timer of type FTimerManager& and Assign it to the return value of GetWorldTimerManager()
    FTimerManager& Timer = GetWorldTimerManager();
    //CALL SetTimer(..) on the Timer variable and pass in -> SpawnTimer, this, &AActorSpawner::SpawnActor, SpawnDelay, true
    Timer.SetTimer(SpawnTimer, this, &AActorSpawner::SpawnActor, SpawnDelay, true);
    // SpawnTimer can now be reused for any other Timer.
    //GetWorldTimerManager().ClearTimer(SpawnTimer);
     
}

// Called every frame
void AActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorSpawner::SpawnActor()
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
            ABall* SpawnedActor = World->SpawnActor<ABall>(BallTemplate, SpawnTransform, SpawnParams);
	    //IF SpawnedActor NOT EQUAL to nullptr
            if (SpawnedActor)
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

