// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWeapon.h"
#include "ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/TextBlock.h"

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

	_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = _RootComponent;

	// Create the box collision and attach it to the root
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	BoxCollisionComponent->InitBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxCollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxCollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	BoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnBoxBeginOverlap);
	BoxCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseWeapon::OnMyComponentEndOverlap);
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter);
    if (OtherActor && OtherActor->IsA<AZombieGameCharacter>())
    {
        if (Character->MainWidgetInstance)
        {
            Overlapping = true;
            FString Text = FString::Printf(TEXT("Press 'E' To Buy %s [Price: %s]"), *Name, *ItemPrice);
            Character->MainWidgetInstance->EquipItemText->SetText(FText::FromString(Text));
            Character->MainWidgetInstance->EquipItemText->SetVisibility(ESlateVisibility::Visible);
			Character->OverlappingWeapon = Cast<ABaseWeapon>(OverlappedComponent->GetOwner());
			UE_LOG(LogTemp, Warning, TEXT("Overlapped Component: %s"), *OverlappedComponent->GetOwner()->GetName());
        }
    }
}


void ABaseWeapon::OnMyComponentEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->MainWidgetInstance)
	{
		Overlapping = false;
		Character->MainWidgetInstance->EquipItemText->SetVisibility(ESlateVisibility::Hidden);
	}
}