// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieGame/Characters/Turret/Turret.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATurret::ATurret()
{
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	DefaultSceneRoot->SetupAttachment(RootComponent);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(DefaultSceneRoot);

	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower Mesh"));
	TowerMesh->SetupAttachment(BaseMesh);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(TowerMesh);

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel Mesh"));
	BarrelMesh->SetupAttachment(GunMesh);

	BulletSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("Bullet Spawn"));
	BulletSpawn->SetupAttachment(BarrelMesh);

	// PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	// PawnSensingComponent->OnSeePawn.AddDynamic(this, &ATurret::OnSeePawn);
}


