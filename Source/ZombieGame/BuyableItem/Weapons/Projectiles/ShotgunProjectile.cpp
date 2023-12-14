// Fill out your copyright notice in the Description page of Project Settings.

#include "ShotgunProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"

AShotgunProjectile::AShotgunProjectile()
{
    HeadDamage = 50;
    TorsoDamage = 30;
    LegDamage = 20;
    ArmDamage = 20;
}

void AShotgunProjectile::HandleBuyItem()
{
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter);

    for (ABaseWeapon* Weapon : Character->Weapons)
    {
        if (Weapon->WeaponType == EWeaponType::E_Shotgun)
        {
            Weapon->TotalWeaponAmmo = Weapon->MaxWeaponAmmo;
        }
    }
}








