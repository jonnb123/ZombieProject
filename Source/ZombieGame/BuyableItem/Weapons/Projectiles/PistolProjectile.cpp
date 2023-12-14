// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"

APistolProjectile::APistolProjectile()
{
    HeadDamage = 20;
    TorsoDamage = 15; 
    LegDamage = 10;
    ArmDamage = 10;
}

void APistolProjectile::HandleBuyItem()
{
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter);

    for (ABaseWeapon* Weapon : Character->Weapons)
    {
        if (Weapon->WeaponType == EWeaponType::E_Pistol)
        {
            Weapon->TotalWeaponAmmo = Weapon->MaxWeaponAmmo;
        }
    }
}










