// Fill out your copyright notice in the Description page of Project Settings.

#include "BuyableItem.h"
#include "ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BaseWeapon.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Algo/Sort.h"
#include "Camera/CameraComponent.h"

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
		IncreaseFireRate();
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

void ABuyableItem::OnMyComponentEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->MainWidgetInstance)
	{
		Character->MainWidgetInstance->EquipItemText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABuyableItem::IncreaseFireRate()
{
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	ABaseWeapon *Pistol = Character->Weapons[0];
	Pistol->FireRate = 0.1;
	FireRateComplete = true;
	Character->Points -= 100;
}

void ABuyableItem::PlayConsumeAnimation()
{
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	Character->AnimationCameraComponent->SetActive(true);
	Character->FirstPersonCameraComponent->SetActive(false);
	// UStaticMeshComponent *MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
	MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());


	// Set the static mesh using the exposed property
	if (PerkBottle)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		// GunMesh->AttachToComponent(Mesh1P, AttachmentRules, TEXT("PistolSocket"));
		MeshComponent->SetStaticMesh(PerkBottle);
		MeshComponent->AttachToComponent(Character->Mesh1P, FAttachmentTransformRules::KeepRelativeTransform, TEXT("BottleSocket"));
		MeshComponent->RegisterComponent();
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		
		if (DrinkingAnimationSequence)
		{
			Character->Mesh1P->PlayAnimation(DrinkingAnimationSequence, false);
			// delay of 2 seconds
			PlayAfterDelayDelegate.BindUObject(this, &ABuyableItem::PlayConsumeAnimationSecondHalf);
			GetWorldTimerManager().SetTimer(PlayAfterDelayHandle, PlayAfterDelayDelegate, 2.0f, false);
		}
	}
}

void ABuyableItem::PlayConsumeAnimationSecondHalf()
{
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	MeshComponent->DestroyComponent();
	Character->FirstPersonCameraComponent->SetActive(true);
	Character->AnimationCameraComponent->SetActive(false);
	Character->Mesh1P->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}

void ABuyableItem::ExtendMagazine()
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
