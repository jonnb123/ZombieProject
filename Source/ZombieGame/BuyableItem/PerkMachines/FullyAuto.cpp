// Fill out your copyright notice in the Description page of Project Settings.

#include "FullyAuto.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"

void AFullyAuto::UseBuyableItem()
{
    UE_LOG(LogTemp, Warning, TEXT("You have interacted with fully auto"));
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->GetPoints() >= 100 && FireRateComplete == false) // set the points to be 1000
	{
		IncreaseFireRate();
		PlayConsumeAnimation();
	}
}

void AFullyAuto::IncreaseFireRate()
{
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	ABaseWeapon *Pistol = Character->Weapons[0];
	Pistol->WeaponFireRate = 0.1;
	FireRateComplete = true;
	Character->SetPoints(Character->GetPoints() - 100);
}


