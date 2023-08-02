// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieGameProjectile.h"
#include "ShotgunProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API AShotgunProjectile : public AZombieGameProjectile
{
	GENERATED_BODY()

public:
	AShotgunProjectile();

	void OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit) override;
};
