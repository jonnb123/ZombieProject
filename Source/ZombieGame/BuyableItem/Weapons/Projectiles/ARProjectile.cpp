// Fill out your copyright notice in the Description page of Project Settings.


#include "ARProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"


AARProjectile::AARProjectile()
{
    HeadDamage = 20;
    TorsoDamage = 15; 
    LegDamage = 10;
    ArmDamage = 10;
}

void AARProjectile::HandleBuyItem()
{
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter);

    for (ABaseWeapon* Weapon : Character->Weapons)
    {
        if (Weapon->WeaponType == EWeaponType::E_AssaultRifle)
        {
            Weapon->TotalWeaponAmmo = Weapon->MaxWeaponAmmo;
        }
    }
}







