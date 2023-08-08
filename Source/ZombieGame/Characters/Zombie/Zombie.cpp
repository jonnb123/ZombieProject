// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieGame/GameMode/ZombieGameMode.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AZombie::AZombie()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create and attach the audio component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	HeadBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadBoxCollisionComponent"));
	HeadBoxCollisionComponent->SetupAttachment(GetMesh());
	HeadBoxCollisionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("head"));

	TorsoBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TorsoBoxCollisionComponent"));
	TorsoBoxCollisionComponent->SetupAttachment(GetMesh());
	TorsoBoxCollisionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("spine_01"));

	RightLegBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RightLegBoxCollisionComponent"));
	RightLegBoxCollisionComponent->SetupAttachment(GetMesh());
	RightLegBoxCollisionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("calf_r"));

	RightArmBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RightArmBoxCollisionComponent"));
	RightArmBoxCollisionComponent->SetupAttachment(GetMesh());
	RightArmBoxCollisionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("lowerarm_r"));

	LeftLegBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftLegBoxCollisionComponent"));
	LeftLegBoxCollisionComponent->SetupAttachment(GetMesh());
	LeftLegBoxCollisionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("calf_l"));

	LeftArmBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftArmBoxCollisionComponent"));
	LeftArmBoxCollisionComponent->SetupAttachment(GetMesh());
	LeftArmBoxCollisionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("lowerarm_l"));
}

float AZombie::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
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

	// Bind the OnBoxBeginOverlap function to the OnComponentBeginOverlap event
	RightArmBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnRightArmBoxBeginOverlap);
	HeadBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnHeadBoxBeginOverlap);
	TorsoBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnTorsoBoxBeginOverlap);
	RightLegBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnRightLegBoxBeginOverlap);
	LeftLegBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnLeftLegBoxBeginOverlap);
	LeftArmBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnLeftArmBoxBeginOverlap);

}

void AZombie::OnHeadBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor);
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Projectile)
	{
		FHitResult Hit;
		FPointDamageEvent DamageEvent(Projectile->HeadDamage, Hit, FVector::ZeroVector, nullptr);
		TakeDamage(Projectile->HeadDamage, DamageEvent, GetInstigatorController(), this);
		HeadHealth -= Projectile->HeadDamage;
		Character->SetPoints(Character->GetPoints() + 50);
		// Get the location of the detached head bone
		FVector DetachedHeadLocation = GetMesh()->GetSocketLocation(TEXT("head"));
		if (HeadHealth <= 0)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ExplodeFX,
				DetachedHeadLocation, // Use the location of the detached head bone
				FRotator::ZeroRotator // You can adjust the rotation as needed
			);
			GetMesh()->HideBoneByName(TEXT("head"), PBO_None);
			HeadBoxCollisionComponent->DestroyComponent();
		}
	}
}

//
void AZombie::OnRightArmBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
										int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor);
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Projectile)
	{
		FHitResult Hit;
		FPointDamageEvent DamageEvent(Projectile->ArmDamage, Hit, FVector::ZeroVector, nullptr);
		TakeDamage(Projectile->ArmDamage, DamageEvent, GetInstigatorController(), this);
		RightArmHealth -= Projectile->ArmDamage;
		// Get the location of the detached arm bone
		FVector DetachedArmLocation = GetMesh()->GetSocketLocation(TEXT("lowerarm_r"));
		if (RightArmHealth <= 0 && LeftArmHealth != 0)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ExplodeFX,
				DetachedArmLocation,  // Use the location of the detached head bone
				FRotator::ZeroRotator // You can adjust the rotation as needed
			);
			GetMesh()->HideBoneByName(TEXT("lowerarm_r"), PBO_None);
			RightArmBoxCollisionComponent->DestroyComponent();
		}
	}
}

void AZombie::OnLeftArmBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									   int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor);
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Projectile)
	{
		FHitResult Hit;
		FPointDamageEvent DamageEvent(Projectile->ArmDamage, Hit, FVector::ZeroVector, nullptr);
		TakeDamage(Projectile->ArmDamage, DamageEvent, GetInstigatorController(), this);
		LeftArmHealth -= Projectile->ArmDamage;
		FVector DetachedArmLocation = GetMesh()->GetSocketLocation(TEXT("lowerarm_l"));
		if (LeftArmHealth <= 0 && RightArmHealth != 0)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ExplodeFX,
				DetachedArmLocation,  // Use the location of the detached head bone
				FRotator::ZeroRotator // You can adjust the rotation as needed
			);
			GetMesh()->HideBoneByName(TEXT("lowerarm_l"), PBO_None);
			LeftArmBoxCollisionComponent->DestroyComponent();
		}
	}
}

void AZombie::OnTorsoBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									 int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor);
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Projectile)
	{
		FHitResult Hit;
		FPointDamageEvent DamageEvent(Projectile->TorsoDamage, Hit, FVector::ZeroVector, nullptr);
		TakeDamage(Projectile->TorsoDamage, DamageEvent, GetInstigatorController(), this);
		TorsoHealth -= Projectile->TorsoDamage;
	}
}

void AZombie::OnRightLegBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
										int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor);
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);

	if (Projectile)
	{
		FHitResult Hit;
		FPointDamageEvent DamageEvent(Projectile->LegDamage, Hit, FVector::ZeroVector, nullptr);
		TakeDamage(Projectile->LegDamage, DamageEvent, GetInstigatorController(), this);
		LegHealth -= Projectile->LegDamage;
		FVector DetachedLeftLegLocation = GetMesh()->GetSocketLocation(TEXT("calf_l"));
		FVector DetachedRightLegLocation = GetMesh()->GetSocketLocation(TEXT("calf_r"));
		if (LegHealth <= 0)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ExplodeFX,
				DetachedLeftLegLocation, // Use the location of the detached head bone
				FRotator::ZeroRotator	 // You can adjust the rotation as needed
			);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ExplodeFX,
				DetachedRightLegLocation, // Use the location of the detached head bone
				FRotator::ZeroRotator	  // You can adjust the rotation as needed
			);
			// hide both legs!
			GetMesh()->HideBoneByName(TEXT("calf_l"), PBO_None);
			GetMesh()->HideBoneByName(TEXT("calf_r"), PBO_None);
			LeftLegBoxCollisionComponent->DestroyComponent();
			RightLegBoxCollisionComponent->DestroyComponent();
			IsCrawling = true;
		}
	}
}

void AZombie::OnLeftLegBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									   int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor);
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);

	if (Projectile)
	{
		FHitResult Hit;
		FPointDamageEvent DamageEvent(Projectile->LegDamage, Hit, FVector::ZeroVector, nullptr);
		TakeDamage(Projectile->LegDamage, DamageEvent, GetInstigatorController(), this);
		LegHealth -= Projectile->LegDamage;
		FVector DetachedLeftLegLocation = GetMesh()->GetSocketLocation(TEXT("calf_l"));
		FVector DetachedRightLegLocation = GetMesh()->GetSocketLocation(TEXT("calf_r"));
		if (LegHealth <= 0)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ExplodeFX,
				DetachedLeftLegLocation, // Use the location of the detached head bone
				FRotator::ZeroRotator	 // You can adjust the rotation as needed
			);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ExplodeFX,
				DetachedRightLegLocation, // Use the location of the detached head bone
				FRotator::ZeroRotator	  // You can adjust the rotation as needed
			);
			// hide both legs!
			GetMesh()->HideBoneByName(TEXT("calf_l"), PBO_None);
			GetMesh()->HideBoneByName(TEXT("calf_r"), PBO_None);
			LeftLegBoxCollisionComponent->DestroyComponent();
			RightLegBoxCollisionComponent->DestroyComponent();
			IsCrawling = true;
		}
	}
}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AZombie::Death()
{
	IsDead = true;

	this->GetCharacterMovement()->StopMovementImmediately();

	AudioComponent->SetComponentTickEnabled(false);
	AudioComponent->Stop();

	AZombieGameMode *MyMode = Cast<AZombieGameMode>(UGameplayStatics::GetGameMode(GetWorld())); // gets the game mode

	MyMode->ZombiesKilled();
}
