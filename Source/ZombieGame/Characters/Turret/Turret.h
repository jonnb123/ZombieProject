// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Perception/PawnSensingComponent.h"
#include "ZombieGame/BuyableItem/Weapons/Projectiles/ZombieGameProjectile.h"
#include "Turret.generated.h"

UCLASS()
class ZOMBIEGAME_API ATurret : public APawn
{
	GENERATED_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
    USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere)
    UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere)
    UStaticMeshComponent* TowerMesh;

	UPROPERTY(EditAnywhere)
    UStaticMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere)
    UStaticMeshComponent* BarrelMesh;

	

	UPROPERTY(EditAnywhere)
	UPawnSensingComponent* PawnSensingComponent;

	

private:	
	// UFUNCTION()
	// void OnSeePawn(APawn* Pawn);


public:
	// Sets default values for this pawn's properties
	ATurret();

	// needed in ZombieGameCharacter
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AZombieGameProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
    USceneComponent* BulletSpawn;
};
