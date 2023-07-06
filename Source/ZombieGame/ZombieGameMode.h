// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZombieGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API AZombieGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void SpawnZombies();

	UFUNCTION(BlueprintImplementableEvent) // doesn't need to be implemented here, all code for this is in unreal
	void WaveStart();

	void ZombiesKilled();

	void WaveIncrement();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerWins();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnAmmo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentWave = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ZombieTotal = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ZombiesLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxWaves = 100; // max number of 8 zombies per random point, so creating more spots will help.

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TSubclassOf<APawn> ZombiePawn;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TSubclassOf<APawn> FireZombiePawn;

	FTimerHandle TimerHandle;

	
};
