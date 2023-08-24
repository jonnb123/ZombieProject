// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"
#include "Turret.generated.h"


UCLASS()
class ZOMBIEGAME_API ATurret : public AActor
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

	ATurret();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> ProjectileBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TurretMesh;

private:
	
	void CheckFireCondition();
	void Fire();

	AZombie* GetClosestZombie();

	TArray<AActor*> Zombies;

	UPROPERTY(EditDefaultsOnly)
	float TurretFireRate;

	UPROPERTY(EditDefaultsOnly)
	float FireRange;

	UPROPERTY(EditDefaultsOnly)
	float Health;

};
