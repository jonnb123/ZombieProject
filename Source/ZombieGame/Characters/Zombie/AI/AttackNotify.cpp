// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieGame/Characters/Zombie/AI/AttackNotify.h"
#include "Engine.h"

void UAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    GEngine->AddOnScreenDebugMessage(-1,4.5f, FColor::Magenta, __FUNCTION__);
    AttackEnd.Broadcast();
}
