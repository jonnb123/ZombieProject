// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "Turret.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimSequence.h"
#include "BuyableItem.generated.h"

UCLASS()
class ZOMBIEGAME_API ABuyableItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuyableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// These are all from the IInteractionInterface that are overriden
	void HealthJuice() override;
	void FullyAuto() override;
	void MaxSpeed() override;
	void ExtendedMag() override;
	void AddTurret() override;


	// The name of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FString Name;

	// Price of the perk
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Price")
	FString ItemPrice;

	UFUNCTION(BlueprintCallable)
	void IncreaseFireRate();

	UFUNCTION(BlueprintCallable)
	void ExtendMagazine();

	UFUNCTION(BlueprintCallable)
	void PlayConsumeAnimation();

	UFUNCTION(BlueprintCallable)
	void PlayConsumeAnimationSecondHalf();

	FTimerHandle PlayAfterDelayHandle;

	FTimerDelegate PlayAfterDelayDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMesh* PerkBottle;

	UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    UAnimSequence* DrinkingAnimationSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complete")
	bool FireRateComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complete")
	bool ExtendedMagComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complete")
	bool TurretComplete = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SpawnRotation;

	// The class of the turret that will be spawned
    UPROPERTY(EditAnywhere)
    TSubclassOf<ATurret> TurretClass;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PerkMachineMesh;

	// Child box collision component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxCollisionComponent;

	// Function to handle the begin overlap event
    UFUNCTION()
    void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    void OnMyComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY()
    ABuyableItem* OverlappingBuyableItem;

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* _RootComponent;

    // // The location at which the turret will be spawned
    UPROPERTY(EditAnywhere, Category = "Turret")
    USceneComponent* TurretSpawnLocation;
};
