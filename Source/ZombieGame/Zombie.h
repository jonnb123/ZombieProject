// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
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

	UPROPERTY(VisibleAnywhere, Category = Health)
	float Health;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float HeadHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float TorsoHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float LeftArmHealth = 50;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float RightArmHealth = 50;

	// UPROPERTY(VisibleAnywhere, Category = Health)
	// float LeftLegHealth = 100;

	// UPROPERTY(VisibleAnywhere, Category = Health)
	// float RightLegHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float LegHealth = 50;

	UPROPERTY(VisibleAnywhere)
	bool IsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsCrawling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    UAudioComponent* AudioComponent;

	// head component with overlap
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    // USkeletalMeshComponent* HeadComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* HeadBoxCollisionComponent;
	UFUNCTION()
    void OnHeadBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// right arm component with overlap
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    // USkeletalMeshComponent* RightArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* RightArmBoxCollisionComponent;
	UFUNCTION()
    void OnRightArmBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// torso component with overlap
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    // USkeletalMeshComponent* TorsoComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* TorsoBoxCollisionComponent;
	UFUNCTION()
    void OnTorsoBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	// right leg component with overlap
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    // USkeletalMeshComponent* RightLegComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* RightLegBoxCollisionComponent;
	UFUNCTION()
    void OnRightLegBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// left leg component with overlap
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    // USkeletalMeshComponent* LeftLegComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* LeftLegBoxCollisionComponent;
	UFUNCTION()
    void OnLeftLegBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// left arm component with overlap
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    // USkeletalMeshComponent* LeftArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* LeftArmBoxCollisionComponent;
	UFUNCTION()
    void OnLeftArmBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTimerEnd();

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraSystem* HeadshotFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraSystem* ArmFX;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Death();

};
