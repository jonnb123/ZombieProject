// Fill out your copyright notice in the Description page of Project Settings.

#include "ShotgunProjectile.h"

AShotgunProjectile::AShotgunProjectile()
{
    HeadDamage = 50;
    TorsoDamage = 30;
    LegDamage = 20;
    ArmDamage = 20;
}

void AShotgunProjectile::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    Destroy();
}
