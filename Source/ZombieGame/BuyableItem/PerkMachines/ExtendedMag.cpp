// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtendedMag.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"

void AExtendedMag::HandleBuyItem()
{
	UE_LOG(LogTemp, Warning, TEXT("You have interacted with fully auto"));
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	
	if (Character->GetPoints() >= FCString::Atoi(*ItemPrice) && ExtendedMagComplete == false) // set the points to be 1000
	{
		ExtendMagazine(); 
		PlayConsumeAnimation();
	}
}

void AExtendedMag::ExtendMagazine()
{
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	TArray<AActor *> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseWeapon::StaticClass(), FoundActors);
	TArray<ABaseWeapon *> FoundWeapons;
	for (AActor *Actor : FoundActors)
	{
		ABaseWeapon *Weapon = Cast<ABaseWeapon>(Actor);
		if (Weapon)
		{
			FoundWeapons.Add(Weapon);
		}
	}
	FoundWeapons[0]->MaxWeaponClipSize = 18;
	FoundWeapons[0]->CurrentWeaponAmmo = 18;
	FoundWeapons[1]->MaxWeaponClipSize = 60;
	FoundWeapons[1]->CurrentWeaponAmmo = 60;
	FoundWeapons[2]->MaxWeaponClipSize = 10;
	FoundWeapons[2]->CurrentWeaponAmmo = 10;
	ExtendedMagComplete = true;
	Character->SetPoints(Character->GetPoints() - FCString::Atoi(*ItemPrice));
}
