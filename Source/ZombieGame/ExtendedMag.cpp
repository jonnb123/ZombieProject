// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedMag.h"
#include "ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BaseWeapon.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Algo/Sort.h"
#include "Camera/CameraComponent.h"


void AExtendedMag::UseBuyableItem()
{
    UE_LOG(LogTemp, Warning, TEXT("You have interacted with fully auto"));
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->Points >= 100 && ExtendedMagComplete == false) // set the points to be 1000
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
	FoundWeapons[0]->MaxClipSize = 18;
	FoundWeapons[0]->CurrentAmmo = 18;
	FoundWeapons[1]->MaxClipSize = 60;
	FoundWeapons[1]->CurrentAmmo = 60;
	FoundWeapons[2]->MaxClipSize = 10;
	FoundWeapons[2]->CurrentAmmo = 10;
	ExtendedMagComplete = true;
	Character->Points -= 100;
}
