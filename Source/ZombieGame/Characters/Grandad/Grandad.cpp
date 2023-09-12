// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieGame/Characters/Grandad/Grandad.h"
#include "ZombieGame/GameMode/ZombieGameMode.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"

// Initialize the Singleton instance - this is done outside of any function
AGrandad *AGrandad::GrandadInstance = nullptr;

// Sets default values
AGrandad::AGrandad()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the box collision and attach it to the root
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	BoxCollisionComponent->InitBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxCollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxCollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ShopCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	ShopCameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AGrandad::BeginPlay()
{
	Super::BeginPlay();

	BoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AGrandad::OnItemBeginOverlap);

	// The only way I've found to get an exact instance of an actor spawned in the world
	TArray<AActor *> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("GrandadTag")), FoundActors);
	GrandadInstance = Cast<AGrandad>(FoundActors[0]);
}

void AGrandad::OnItemBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
								  int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("OVERLAPPING WITH GRANDAD"));
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	APlayerController *PlayerController = Cast<APlayerController>(Character->GetController());

	if (OtherActor->IsA<AZombieGameCharacter>())
	{
		PlayerController->SetViewTargetWithBlend(this, 2.0);

		if (MainWidgetInstance)
		{
			MainWidgetInstance->RemoveFromParent();
		}

		ShopWidgetInstance = CreateWidget<UShopWidget>(GetWorld(), ShopWidgetClass);
		ShopWidgetInstance->AddToViewport();
		// PlayerController->SetInputMode(FInputModeUIOnly());
		// PlayerController->bShowMouseCursor = true;
		// GetCharacterMovement()->StopMovementImmediately();
	}
}

// Static method to get the Singleton instance
AGrandad *AGrandad::GetInstance()
{
	return GrandadInstance;
}

// Called every frame
void AGrandad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGrandad::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AGrandad::TakeDamage(float const DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); // takes the damage values from the zombie in BTTask_Attack and plugs them into the base implementation of TakeDamage
	if (Health <= 0)
	{
		// when health is below 0
		UE_LOG(LogTemp, Log, TEXT("Health left %f"), Health);
		ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
		Character->HandleCharacterDeath();
	}
	else // player is alive
	{
		DamageToApply = FMath::Min(Health, DamageToApply);
		Health -= DamageToApply; // deducts damage from health
		UE_LOG(LogTemp, Log, TEXT("Health left %f"), Health);
	}
	return DamageToApply;
}

void AGrandad::HandleDeath()
{
	IsDead = true;
	AZombieGameMode *MyMode = Cast<AZombieGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainWidgetInstance->ShowDeathWindow();
}
