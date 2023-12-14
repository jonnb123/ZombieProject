// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWeapon.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"

void ABaseWeapon::HandleBuyItem()
{
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);

	if (IsObtained == false && Character->GetPoints() >= FCString::Atoi(*ItemPrice))
	{
		Character->SetPoints(Character->GetPoints() - FCString::Atoi(*ItemPrice));
		IsObtained = true;
		Character->Weapons.AddUnique(this);
		if (Character->Weapons.Num() - 1 != -1) // if the newly added weapons index isn't -1
		{
			Character->SwitchToNextPrimaryWeapon();
		}
	}
}

// void ABaseWeapon::UseBuyableItem()
// {
// 	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
// 	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
//
// 	if (IsObtained == false && Character->GetPoints() >= FCString::Atoi(*ItemPrice))
// 	{
// 		Character->SetPoints(Character->GetPoints() - FCString::Atoi(*ItemPrice));
// 		IsObtained = true;
// 		Character->Weapons.AddUnique(this);
// 		if (Character->Weapons.Num() - 1 != -1) // if the newly added weapons index isn't -1
// 		{
// 			Character->SwitchToNextPrimaryWeapon();
// 		}
// 	}
// }
