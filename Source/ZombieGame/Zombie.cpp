// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieGameMode.h"
#include "PawnSensingComponent.generated.h"



// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create and attach the audio component
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(RootComponent);
}

float AZombie::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) 
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (IsDead == false)
	{
		DamageToApply = FMath::Min(Health, DamageToApply);
		Health -= DamageToApply;
		// if alive
		if (Health > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);
			this->PlayAnimMontage(HitMontage);
			ZombieHitCheck = true;
			return DamageToApply;
		}
		// when health is below 0
		UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);
		Death();
		this->SetActorEnableCollision(false);
		return DamageToApply;
	}
	return DamageToApply; // DamageToApply just needs to be in function, not sure why
	
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth; // sets health equal to 100
}

void AZombie::OnTimerEnd() // not necessary for interview
{
	UE_LOG(LogTemp, Warning, TEXT("Timer is up"));
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AZombie::Death() 
{
	IsDead = true;

	this->GetCharacterMovement()->StopMovementImmediately();

	AudioComponent->SetComponentTickEnabled(false);
	AudioComponent->Stop();

	this->PlayAnimMontage(AnimMontage);

	AZombieGameMode* MyMode = Cast<AZombieGameMode>(UGameplayStatics::GetGameMode(GetWorld())); // gets the game mode

	MyMode->ZombiesKilled();

	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AZombie::OnTimerEnd, 10.f, false);
	
	// this->Destroy();
}

