// Fill out your copyright notice in the Description page of Project Settings.


#include "ARProjectile.h"


AARProjectile::AARProjectile()
{
    HeadDamage = 20;
    TorsoDamage = 15; 
    LegDamage = 10;
    ArmDamage = 10;
}

void AARProjectile::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    Destroy();
}
