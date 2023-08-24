// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "ZombieGameProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config = Game)
class AZombieGameProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Public getter because turret also uses this projectile class
	UProjectileMovementComponent *GetProjectileMovement() const { return ProjectileMovement; }

	// Need to be accessed in Zombie.cpp
	UPROPERTY(EditAnywhere, Category = "Damage")
	float HeadDamage;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float TorsoDamage;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float ArmDamage;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float LegDamage;

protected:
	AZombieGameProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);


private:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UBoxComponent *CollisionComponent;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent *ProjectileMovement;


};
