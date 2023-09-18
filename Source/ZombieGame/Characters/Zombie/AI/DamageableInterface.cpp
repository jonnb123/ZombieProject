// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieGame/Characters/Zombie/AI/DamageableInterface.h"

// Add default functionality here for any IDamageableInterface functions that are not pure virtual.

void IDamageableInterface::HandleDamage(float const DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
    UE_LOG(LogTemp, Error, TEXT("Take damage not implemented for..."));
}
