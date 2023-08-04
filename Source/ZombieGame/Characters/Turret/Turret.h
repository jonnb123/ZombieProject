// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
// #include "Zombie.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"
#include "Turret.generated.h"


UCLASS()
class ZOMBIEGAME_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Fire();

	FHitResult HitResult;

	// maximum range
	UPROPERTY(EditAnywhere)
	float MaxBulletRange = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> ProjectileBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* GunMesh;


private:
	void CheckFireCondition();
	AZombie* GetClosestZombie();
	AController* Instigator;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TurretMesh;

	
	UPROPERTY(EditDefaultsOnly)
	float FireRate;

	UPROPERTY(EditDefaultsOnly)
	float FireRange;

	UPROPERTY(EditDefaultsOnly)
	float Health;

	UPROPERTY()
	TArray<AActor*> Zombies;

	UPROPERTY()
	FTimerHandle FireRateTimerHandle;

};
