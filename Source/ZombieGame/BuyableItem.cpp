// Fill out your copyright notice in the Description page of Project Settings.

#include "BuyableItem.h"
#include "ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BaseWeapon.h"


// Sets default values
ABuyableItem::ABuyableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = _RootComponent;

	PerkMachineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Perk Machine Mesh"));
	PerkMachineMesh->SetupAttachment(RootComponent);

	Name = "Health Juice";

	ItemPrice = "2000";

	FireRateComplete = false;
}

// Called when the game starts or when spawned
void ABuyableItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuyableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuyableItem::HealthJuice() 
{
	UE_LOG(LogTemp, Warning, TEXT("You have interacted with health juice"));
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->Points >= 100 && Character->Health <=100) // set the points to be 2000
	{
		Character->Points -= 100; // set the points to be 2000
		Character->Health = 200;
		PlayConsumeAnimation();
	}
}

void ABuyableItem::FullyAuto() 
{
	UE_LOG(LogTemp, Warning, TEXT("You have interacted with fully auto"));
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->Points >= 100 && FireRateComplete == false) // set the points to be 1000
	{
		IncreaseFireRate(); // functionality in blueprint 
		PlayConsumeAnimation();
	}
}

void ABuyableItem::ExtendedMag() 
{
	UE_LOG(LogTemp, Warning, TEXT("You have interacted with fully auto"));
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->Points >= 100 && ExtendedMagComplete == false) // set the points to be 1000
	{
		ExtendMagazine(); // functionality in blueprint 
		PlayConsumeAnimation();
	}
}

void ABuyableItem::AddTurret() 
{
	UE_LOG(LogTemp, Warning, TEXT("You have interacted with turret"));
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	// Get the world
    UWorld* const World = GetWorld();
	// Check if the player has enough points to buy the turret
    if (Character->Points >= 100 && TurretComplete == false)
    {
		PlayConsumeAnimation();
        // Spawn the turret at the specified location
        AActor* Turret = World->SpawnActor(TurretClass, &SpawnLocation, &SpawnRotation);
        if (Turret)
        {
            Character->Points -= 100;
			TurretComplete = true;
        }
    }
}

void ABuyableItem::MaxSpeed() 
{
	UE_LOG(LogTemp, Warning, TEXT("You have interacted with Max Speed"));
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->Points >= 100 && Character->GetCharacterMovement()->MaxWalkSpeed < 1200.0f) // set the points to be 1000
	{
		Character->Points -= 100;
		Character->GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
		PlayConsumeAnimation();
	}
}



