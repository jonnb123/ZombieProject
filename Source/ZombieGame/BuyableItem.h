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
class ZOMBIEGAME_API ABuyableItem : public AActor
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

	// The name of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FString Name;

	// Price of the perk
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Price")
	FString ItemPrice;

	FTimerHandle PlayAfterDelayHandle;

	FTimerDelegate PlayAfterDelayDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMesh* PerkBottle;

	UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    UAnimSequence* DrinkingAnimationSequence;

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

	UFUNCTION(BlueprintCallable)
	virtual void UseBuyableItem();

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* _RootComponent;

    // // // The location at which the turret will be spawned
    // UPROPERTY(EditAnywhere, Category = "Turret")
    // USceneComponent* TurretSpawnLocation;
};
