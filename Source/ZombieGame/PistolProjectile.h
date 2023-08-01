// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieGameProjectile.h"
#include "PistolProjectile.generated.h"

/**
 *
 */
UCLASS()
class ZOMBIEGAME_API APistolProjectile : public AZombieGameProjectile
{
	GENERATED_BODY()

public:
	APistolProjectile();

	void OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit) override;

};
