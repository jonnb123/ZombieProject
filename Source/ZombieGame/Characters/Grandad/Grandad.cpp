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
}

// Called when the game starts or when spawned
void AGrandad::BeginPlay()
{
	Super::BeginPlay();
}

// // Static method to get the Singleton instance
// AGrandad* AGrandad::GetInstance()
// {
//     if (GrandadInstance == nullptr)
//     {
//         GrandadInstance = NewObject<AGrandad>();
//     }
//     return GrandadInstance;
// }

// Static method to get the Singleton instance
AGrandad* AGrandad::GetInstance()
{
    if (GrandadInstance == nullptr)
    {
        GrandadInstance = NewObject<AGrandad>();
    }
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
