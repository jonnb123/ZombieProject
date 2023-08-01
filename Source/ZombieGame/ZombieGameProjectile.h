// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieGameProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config = Game)
class AZombieGameProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent *CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent *ProjectileMovement;

public:
	AZombieGameProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent *GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent *GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent *ProjectileCollision;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float HeadDamage;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float TorsoDamage;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float ArmDamage;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float LegDamage;


};
