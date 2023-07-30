// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWeapon.h"
#include "ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentAmmo = 9;
	TotalAmmo = 10;
	MaxClipSize = 9;
	WeaponType = EWeaponType::E_Pistol;
	Index = 0;
	IsObtained = false;
	Overlapping = false;
	Name = "Weapon";
	ItemPrice = "1000";
	FireRate = 0.25f;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWeapon::UseBuyableItem()
{
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);

	if (IsObtained == false && Character->Points >= FCString::Atoi(*ItemPrice))
	{
		Character->Points -= FCString::Atoi(*ItemPrice);
		IsObtained = true;
		Character->Weapons.AddUnique(this);
		if (Character->Weapons.Num() - 1 != -1) // if the newly added weapons index isn't -1
		{
			Character->SwitchToNextPrimaryWeapon();
			// Character->IsSwappingWeapon = true;
			// GetWorldTimerManager().SetTimer(WeaponPickupTimerHandle, this, &ABaseWeapon::WeaponPickupAfterDelay, 2.0f, false);
		}
	}
}
