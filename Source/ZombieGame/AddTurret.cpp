// Fill out your copyright notice in the Description page of Project Settings.


#include "AddTurret.h"
#include "ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BaseWeapon.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Algo/Sort.h"
#include "Camera/CameraComponent.h"

void AAddTurret::UsePerk()
{
    UE_LOG(LogTemp, Warning, TEXT("You have interacted with turret"));
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	// Get the world
	UWorld *const World = GetWorld();
	// Check if the player has enough points to buy the turret
	if (Character->Points >= 100 && TurretComplete == false)
	{
		PlayConsumeAnimation();
		// Spawn the turret at the specified location
		AActor *Turret = World->SpawnActor(TurretClass, &SpawnLocation, &SpawnRotation);
		if (Turret)
		{
			Character->Points -= 100;
			TurretComplete = true;
		}
	}
}
