// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "ZombieGame/Characters/Zombie/AI/DamageableInterface.h"
#include "NiagaraSystem.h"
#include "Perception/AIPerceptionTypes.h"
#include "Zombie.generated.h"



UCLASS()
class ZOMBIEGAME_API AZombie : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();

	// getter and setter for HitCheck
	bool GetZombieHitCheck() const { return ZombieHitCheck; }
	void SetZombieHitCheck(bool NewValue) { ZombieHitCheck = NewValue; }

	// Getter for IsDead
	bool GetIsZombieDead() const { return IsDead; }

	// Getter for IsCrawling
	bool GetIsCrawling() const { return IsCrawling; }

	virtual void HandleDamage(float const DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent *HeadBoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent *RightArmBoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent *TorsoBoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent *RightLegBoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent *LeftLegBoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent *LeftArmBoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX")
	UAudioComponent *AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraSystem *ExplodeFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UAnimMontage *HitMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsDead; // used in abp and zombieaicontroller and turret, only need a getter

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsCrawling = false; // used in abp and zombieaicontroller, and BTTastk_Attack only need a getter

	UPROPERTY(VisibleAnywhere, Category = Health)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float Health = 100;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float HeadHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float TorsoHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float LeftArmHealth = 50;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float RightArmHealth = 50;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float LegHealth = 50;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float NoOppositeLimb = -1; // placeholder for when component such as head doesn't have an opposite limb

	UPROPERTY(EditAnywhere, Category = VFX)
	UNiagaraSystem* ZombieImpactVFX;

private:
	// Declare a common function to handle body part overlap
	void HandleBodyPartOverlap(float Damage, float &LimbHealth, const FName &BoneToRemoveOne, const FName &BoneToRemoveTwo, AActor *OtherActor, UPrimitiveComponent *OverlappedComponent, float &OppositeLimbHealth);

	// On begin overlap functions, DO NOT DELETE UFUNCTION IT'S ESSENTIAL
	UFUNCTION()
	void OnHeadBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
							   int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnRightArmBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
								   int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnLeftArmBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
								  int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnRightLegBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
								   int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnTorsoBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
								int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnLeftLegBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
								  int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnMeshHit(
		UPrimitiveComponent *HitComponent, // The component that was hit
		AActor *OtherActor,				   // The actor that hit the component
		UPrimitiveComponent *OtherComp,	   // The other component involved in the collision
		FVector NormalImpulse,			   // The impulse applied for the hit
		const FHitResult &Hit			   // Information about the hit, including impact point and normal
	);
	// Functions

	bool ZombieHitCheck = false; // also used in ZombieAIController, uses getter and setter

	void Death();

	void DestroyZombieMesh();

	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
};
