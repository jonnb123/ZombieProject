// Fill out your copyright notice in the Description page of Project Settings.

#include "AddTurret.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"

void AAddTurret::UseBuyableItem()
{
    UE_LOG(LogTemp, Warning, TEXT("You have interacted with turret"));
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	// Get the world
	UWorld *const World = GetWorld();
	// Check if the player has enough points to buy the turret
	if (Character->GetPoints() >= 100 && TurretComplete == false)
	{
		PlayConsumeAnimation();
		// Spawn the turret at the specified location
		AActor *Turret = World->SpawnActor(TurretClass, &SpawnLocation, &SpawnRotation);
		if (Turret)
		{
			Character->SetPoints(Character->GetPoints() - 100);
			TurretComplete = true;
		}
	}
}
