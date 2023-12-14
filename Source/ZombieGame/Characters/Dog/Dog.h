// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieGame/Widgets/BuyableItemInterface.h"
#include "Dog.generated.h"

UCLASS()
class ZOMBIEGAME_API ADog : public ACharacter, public IBuyableItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void HandleBuyItem() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> DogPawn;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree *BehaviorTree;

	UPROPERTY(EditAnywhere)
	FVector DogSpawnLocation;

	

};
