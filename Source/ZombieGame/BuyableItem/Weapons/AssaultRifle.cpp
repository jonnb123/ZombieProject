// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultRifle.h"

#include "Kismet/GameplayStatics.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"


// Sets default values
AAssaultRifle::AAssaultRifle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAssaultRifle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAssaultRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAssaultRifle::HandleBuyItem()
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

