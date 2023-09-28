// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZombieGame/Characters/FrontDoor/FrontDoor.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"
#include "ZombieGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API AZombieGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Getter for zombies left
	int GetZombiesLeft() const { return ZombiesLeft; }

	// Getter for zombies left
	int GetCurrentWave() const { return CurrentWave; }

	// Getter for zombies left
	int GetMaxWaves() const { return MaxWaves; }

	// This function is used in Zombie.cpp, so is kept public
	void HandleZombieCountAndRound();

	// A reference to the behaviour tree for the zombies
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BehaviorTree;

	TArray<AZombie*> SpawnedZombies;


protected:
	virtual void BeginPlay() override;

	// All zombies are in protected as they need to be set in blueprint
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TSubclassOf<APawn> ZombiePawn1;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TSubclassOf<APawn> ZombiePawn2;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TSubclassOf<APawn> ZombiePawn3;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TSubclassOf<APawn> ZombiePawn4;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TSubclassOf<APawn> FireZombiePawn;

	
	// uses a getter to be used in widget, protected so the wave can be edited in BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentWave = 5;

	// uses a getter to be used in widget, protected so the wave can be edited in BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxWaves = 100;

	// functionality in blueprint
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnAmmo();

private:
	// functions
	void StartGame();
	void SpawnZombies();
	void HandleWaveStart();
	void WaveIncrement();
	void PlayerWins();
	
	// variables
	int ZombieTotal;
	int ZombiesLeft; // uses a getter, as it's used in MainWidget.cpp
	TArray<TSubclassOf<APawn>> ZombiePawnClasses;
};
