// Fill out your copyright notice in the Description page of Project Settings.

#include "BuyableItem.h"
#include "ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BaseWeapon.h"
#include "UMG/Public/Components/TextBlock.h"

// Sets default values
ABuyableItem::ABuyableItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = _RootComponent;

	PerkMachineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Perk Machine Mesh"));
	PerkMachineMesh->SetupAttachment(RootComponent);

	// Create the box collision and attach it to the root
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	BoxCollisionComponent->InitBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxCollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxCollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Name = "Health Juice";

	ItemPrice = "2000";

	FireRateComplete = false;
}

// Called when the game starts or when spawned
void ABuyableItem::BeginPlay()
{
	Super::BeginPlay();
	BoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABuyableItem::OnBoxBeginOverlap);
	BoxCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ABuyableItem::OnMyComponentEndOverlap);

}

// Called every frame
void ABuyableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABuyableItem::HealthJuice()
{
	UE_LOG(LogTemp, Warning, TEXT("You have interacted with health juice"));
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->Points >= 100 && Character->Health <= 100) // set the points to be 2000
	{
		Character->Points -= 100; // set the points to be 2000
		Character->Health = 200;
		PlayConsumeAnimation();
	}
}

void ABuyableItem::FullyAuto()
{
	UE_LOG(LogTemp, Warning, TEXT("You have interacted with fully auto"));
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->Points >= 100 && FireRateComplete == false) // set the points to be 1000
	{
		IncreaseFireRate(); // functionality in blueprint
		PlayConsumeAnimation();
	}
}

void ABuyableItem::ExtendedMag()
{
	UE_LOG(LogTemp, Warning, TEXT("You have interacted with fully auto"));
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->Points >= 100 && ExtendedMagComplete == false) // set the points to be 1000
	{
		ExtendMagazine(); // functionality in blueprint
		PlayConsumeAnimation();
	}
}

void ABuyableItem::AddTurret()
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

void ABuyableItem::MaxSpeed()
{
	UE_LOG(LogTemp, Warning, TEXT("You have interacted with Max Speed"));
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->Points >= 100 && Character->GetCharacterMovement()->MaxWalkSpeed < 1200.0f) // set the points to be 1000
	{
		Character->Points -= 100;
		Character->GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
		PlayConsumeAnimation();
	}
}

// this function is bound in begin play, without binding it it doesn't work
void ABuyableItem::OnBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									 int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("OVERLAPPING IN CODE"));
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (OtherActor && OtherActor->IsA<AZombieGameCharacter>())
	{
		if (Character->MainWidgetInstance)
		{
			FString Text = FString::Printf(TEXT("Press 'E' To Buy %s [Price: %s]"), *Name, *ItemPrice);
			Character->MainWidgetInstance->EquipItemText->SetText(FText::FromString(Text));
			Character->MainWidgetInstance->EquipItemText->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ABuyableItem::OnMyComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->MainWidgetInstance)
	{
		Character->MainWidgetInstance->EquipItemText->SetVisibility(ESlateVisibility::Hidden);
	}
}
