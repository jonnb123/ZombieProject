// Fill out your copyright notice in the Description page of Project Settings.


#include "FullyAuto.h"
#include "ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BaseWeapon.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Algo/Sort.h"
#include "Camera/CameraComponent.h"


void AFullyAuto::UsePerk()
{
    UE_LOG(LogTemp, Warning, TEXT("You have interacted with fully auto"));
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->Points >= 100 && FireRateComplete == false) // set the points to be 1000
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
	Pistol->FireRate = 0.1;
	FireRateComplete = true;
	Character->Points -= 100;
}


