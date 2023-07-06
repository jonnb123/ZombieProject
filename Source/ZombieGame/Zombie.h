// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "Zombie.generated.h"

UCLASS()
class ZOMBIEGAME_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Determines if the character is currently sprinting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* HitMontage;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ZombieHitCheck = false;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(VisibleAnywhere)
	bool IsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    UAudioComponent* AudioComponent;

	UFUNCTION()
	void OnTimerEnd();

	FTimerHandle TimerHandle;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Death();

};
