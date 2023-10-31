// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieGame/Characters/Turret/Turret.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"
#include "Kismet/KismetMathLibrary.h"

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

	BarrelMeshOne = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel Mesh One"));
	BarrelMeshOne->SetupAttachment(GunMesh);

	BarrelMeshTwo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel Mesh Two"));
	BarrelMeshTwo->SetupAttachment(GunMesh);

	BulletSpawnOne = CreateDefaultSubobject<USceneComponent>(TEXT("Bullet Spawn One"));
	BulletSpawnOne->SetupAttachment(BarrelMeshOne);

	BulletSpawnTwo = CreateDefaultSubobject<USceneComponent>(TEXT("Bullet Spawn Two"));
	BulletSpawnTwo->SetupAttachment(BarrelMeshTwo);

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ATurret::OnSeePawn);
}

void ATurret::OnSeePawn(APawn *Pawn)
{
	UE_LOG(LogTemp, Warning, TEXT("HELLO HI HELP"));

	AZombie *Zombie = Cast<AZombie>(Pawn);

	if (Zombie)
	{
		UE_LOG(LogTemp, Warning, TEXT("WELCOME TO HELL MOTHERFUCKER"));

		FVector ZombieLocation = Zombie->GetActorLocation();

		FVector TurretLocation = GetActorLocation();
		FRotator TurretRotation = GetActorRotation();

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(TurretLocation, ZombieLocation);

		FRotator InterpValue = UKismetMathLibrary::RInterpTo(TurretRotation, LookAtRotation, GetWorld()->GetDeltaSeconds(), 100.0);

		// REMEMBER: PITCH (y), YAW (z), ROLL (x)
		SetActorRotation(FRotator(0,InterpValue.Yaw,0));
	}
}
