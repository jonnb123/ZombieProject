// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ZombieGame/Characters/Zombie/AI/DamageableInterface.h"
#include "ZombieGame/Characters/Zombie/AI/ZombieAIController.h"
#include "ZombieGame/Widgets/BuyableItemInterface.h"
#include "FrontDoor.generated.h"


UCLASS()
class ZOMBIEGAME_API AFrontDoor : public AActor, public IDamageableInterface, public IBuyableItemInterface
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AFrontDoor();

	void UseFrontDoor();

	UPROPERTY(EditAnywhere)
	int Health = 100;

	bool bDestroyed = false;

	bool bDoorOpen = false;

	bool bIsSpawned = false;

	virtual void HandleDamage(float const DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;

	virtual void HandleBuyItem() override;

	// Static method to get the Singleton instance
    static AFrontDoor* GetInstance();

	static void SetInstance(AFrontDoor* NewInstance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AFrontDoor> FrontDoor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DoorSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator DoorSpawnRotation;

	// Child box collision component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxCollisionComponent;
	
	 UFUNCTION()
    void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    void OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* _RootComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorMesh;

	// Static instance of Singleton
	static AFrontDoor* FrontDoorInstance;

	
};
