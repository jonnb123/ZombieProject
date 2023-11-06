// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieGame/GameMode/ZombieGameMode.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"

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

void AZombie::HandleDamage(float const DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (IsDead == false)
	{
		DamageToApply = FMath::Min(Health, DamageToApply);
		Health -= DamageToApply;
		if (Health > 0)
		{
			PlayAnimMontage(HitMontage);
			ZombieHitCheck = true;
			UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);
		}
		else
		{
			Death();
		}
	}
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();

	// Bind the OnBoxBeginOverlap function to the OnComponentBeginOverlap event
	RightArmBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnRightArmBoxBeginOverlap);
	LeftArmBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnLeftArmBoxBeginOverlap);
	HeadBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnHeadBoxBeginOverlap);
	RightLegBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnRightLegBoxBeginOverlap);
	TorsoBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnTorsoBoxBeginOverlap);
	LeftLegBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AZombie::OnLeftLegBoxBeginOverlap);
	GetMesh()->OnComponentHit.AddDynamic(this, &AZombie::OnMeshHit);
}

void AZombie::OnHeadBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor);
	if (Projectile)
	{
		HandleBodyPartOverlap(Projectile->HeadDamage, HeadHealth, TEXT("head"), TEXT("NONE"), OtherActor, OverlappedComponent, NoOppositeLimb);
	}
}

void AZombie::OnRightArmBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
										int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor);
	if (Projectile)
	{
		HandleBodyPartOverlap(Projectile->ArmDamage, RightArmHealth, TEXT("lowerarm_r"), TEXT("NONE"), OtherActor, OverlappedComponent, LeftArmHealth);
	}
}

void AZombie::OnLeftArmBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									   int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor);
	if (Projectile)
	{
		HandleBodyPartOverlap(Projectile->ArmDamage, LeftArmHealth, TEXT("lowerarm_l"), TEXT("NONE"), OtherActor, OverlappedComponent, RightArmHealth);
	}
}

void AZombie::OnRightLegBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
										int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor);
	if (Projectile)
	{
		HandleBodyPartOverlap(Projectile->LegDamage, LegHealth, TEXT("calf_r"), TEXT("calf_l"), OtherActor, OverlappedComponent, NoOppositeLimb);
	}
}

void AZombie::OnLeftLegBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									   int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor);
	if (Projectile)
	{
		HandleBodyPartOverlap(Projectile->LegDamage, LegHealth, TEXT("calf_r"), TEXT("calf_l"), OtherActor, OverlappedComponent, NoOppositeLimb);
	}
}

void AZombie::OnMeshHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Zombie mesh hit"));
	// 	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), )
	OtherComp->DestroyComponent();
}

void AZombie::OnTorsoBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									 int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor);
	if (Projectile)
	{
		// not removing any limb here
		HandleBodyPartOverlap(Projectile->TorsoDamage, TorsoHealth, TEXT("NONE"), TEXT("NONE"), OtherActor, OverlappedComponent, NoOppositeLimb);
	}
}

void AZombie::HandleBodyPartOverlap(float Damage, float &LimbHealth, const FName &BoneToRemoveOne, const FName &BoneToRemoveTwo, AActor *OtherActor, UPrimitiveComponent *OverlappedComponent, float &OppositeLimbHealth)
{
	FHitResult Hit;
	FPointDamageEvent DamageEvent(Damage, Hit, FVector::ZeroVector, nullptr);
	// TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
	IDamageableInterface *TheInterface = Cast<IDamageableInterface>(this);
	if (TheInterface)
	{
		TheInterface->HandleDamage(Damage, DamageEvent, GetInstigatorController(), this);
	}
	// HandleDamage(Damage, DamageEvent, GetInstigatorController(), this);

	LimbHealth -= Damage;
	if (LimbHealth <= 0 && OppositeLimbHealth != 0)
	{
		if (BoneToRemoveTwo == FName(TEXT("NONE")) && BoneToRemoveOne != FName(TEXT("NONE")))
		{
			// Get the location of the detached bone
			FVector DetachedLocation = GetMesh()->GetSocketLocation(BoneToRemoveOne);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ExplodeFX,
				DetachedLocation,
				FRotator::ZeroRotator);
			GetMesh()->HideBoneByName(BoneToRemoveOne, PBO_None);
			OverlappedComponent->DestroyComponent();
		}
		else if (((BoneToRemoveTwo == FName(TEXT("calf_r"))) || BoneToRemoveTwo == FName(TEXT("calf_l"))))
		{
			FVector DetachedLeftLegLocation = GetMesh()->GetSocketLocation(TEXT("calf_l"));
			FVector DetachedRightLegLocation = GetMesh()->GetSocketLocation(TEXT("calf_r"));
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ExplodeFX,
				DetachedLeftLegLocation,
				FRotator::ZeroRotator);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ExplodeFX,
				DetachedRightLegLocation,
				FRotator::ZeroRotator);
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

	GetCharacterMovement()->StopMovementImmediately();

	AudioComponent->SetComponentTickEnabled(false);
	AudioComponent->Stop();
	SetActorEnableCollision(false);

	AZombieGameMode *MyMode = Cast<AZombieGameMode>(UGameplayStatics::GetGameMode(GetWorld())); // gets the game mode

	MyMode->HandleZombieCountAndRound();

	GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, HitMontage);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AZombie::DestroyZombieMesh, 2.0f, false);
}

void AZombie::DestroyZombieMesh()
{
	Destroy();
}
