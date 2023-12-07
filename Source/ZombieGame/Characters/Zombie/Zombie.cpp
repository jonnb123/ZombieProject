// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieGame/GameMode/ZombieGameMode.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"


// Sets default values
AZombie::AZombie()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create and attach the audio component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	HeadBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadBoxCollisionComponent"));
	HeadBoxCollisionComponent->SetupAttachment(GetMesh(), TEXT("head"));
	// HeadBoxCollisionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("head"));

	TorsoBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TorsoBoxCollisionComponent"));
	TorsoBoxCollisionComponent->SetupAttachment(GetMesh(), TEXT("spine_01"));
	// TorsoBoxCollisionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("spine_01"));

	RightLegBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RightLegBoxCollisionComponent"));
	RightLegBoxCollisionComponent->SetupAttachment(GetMesh(), TEXT("calf_r"));
	// RightLegBoxCollisionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("calf_r"));

	RightArmBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RightArmBoxCollisionComponent"));
	RightArmBoxCollisionComponent->SetupAttachment(GetMesh(), TEXT("lowerarm_r"));
	// RightArmBoxCollisionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("lowerarm_r"));

	LeftLegBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftLegBoxCollisionComponent"));
	LeftLegBoxCollisionComponent->SetupAttachment(GetMesh(), TEXT("calf_l"));
	// LeftLegBoxCollisionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("calf_l"));

	LeftArmBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftArmBoxCollisionComponent"));
	LeftArmBoxCollisionComponent->SetupAttachment(GetMesh(), TEXT("lowerarm_l"));
	// LeftArmBoxCollisionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("lowerarm_l"));

	
	UCharacterMovementComponent *ZombieMovement = Cast<UCharacterMovementComponent>(GetMovementComponent());
	ZombieMovement->MaxWalkSpeed = 300.f;

	Health = MaxHealth;
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



void AZombie::OnHeadBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor))
	{
		HandleBodyPartOverlap(Projectile->HeadDamage, HeadHealth, TEXT("head"), TEXT("NONE"), OtherActor, OverlappedComponent, NoOppositeLimb);
		ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
		Character->SetPoints(Character->GetPoints() + 100);
	}
}

void AZombie::OnRightArmBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
										int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor))
	{
		HandleBodyPartOverlap(Projectile->ArmDamage, RightArmHealth, TEXT("lowerarm_r"), TEXT("NONE"), OtherActor, OverlappedComponent, LeftArmHealth);
		ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
		Character->SetPoints(Character->GetPoints() + 50);
	}
}

void AZombie::OnLeftArmBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									   int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor))
	{
		HandleBodyPartOverlap(Projectile->ArmDamage, LeftArmHealth, TEXT("lowerarm_l"), TEXT("NONE"), OtherActor, OverlappedComponent, RightArmHealth);
		ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
		Character->SetPoints(Character->GetPoints() + 50);
	}
}

void AZombie::OnRightLegBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
										int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor))
	{
		HandleBodyPartOverlap(Projectile->LegDamage, LegHealth, TEXT("calf_r"), TEXT("calf_l"), OtherActor, OverlappedComponent, NoOppositeLimb);
		ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
		Character->SetPoints(Character->GetPoints() + 50);
	}
}

void AZombie::OnLeftLegBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									   int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor))
	{
		HandleBodyPartOverlap(Projectile->LegDamage, LegHealth, TEXT("calf_r"), TEXT("calf_l"), OtherActor, OverlappedComponent, NoOppositeLimb);
		ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
		Character->SetPoints(Character->GetPoints() + 50);
	}
}

void AZombie::OnTorsoBoxBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
									 int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (AZombieGameProjectile *Projectile = Cast<AZombieGameProjectile>(OtherActor))
	{
		// not removing any limb here
		HandleBodyPartOverlap(Projectile->TorsoDamage, TorsoHealth, TEXT("NONE"), TEXT("NONE"), OtherActor, OverlappedComponent, NoOppositeLimb);
		ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
		Character->SetPoints(Character->GetPoints() + 50);
	}
}

void AZombie::OnMeshHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Zombie mesh hit"));
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ZombieImpactVFX, Hit.Location);
	// OtherComp->DestroyComponent();
}



void AZombie::HandleBodyPartOverlap(float Damage, float &LimbHealth, const FName &BoneToRemoveOne, const FName &BoneToRemoveTwo, AActor *OtherActor, UPrimitiveComponent *OverlappedComponent, float &OppositeLimbHealth)
{
	FHitResult Hit;
	FPointDamageEvent DamageEvent(Damage, Hit, FVector::ZeroVector, nullptr);
	IDamageableInterface *TheInterface = Cast<IDamageableInterface>(this);
	if (TheInterface)
	{
		TheInterface->HandleDamage(Damage, DamageEvent, GetInstigatorController(), this);
	}

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
	UE_LOG(LogTemp, Warning, TEXT("ZOMBIE DEAD "));

	// AI can no longer detect the zombie upon death
	if (UAIPerceptionStimuliSourceComponent* StimuliSourceComponent = FindComponentByClass<UAIPerceptionStimuliSourceComponent>())
	{
		// Unregister from the Sight sense
		StimuliSourceComponent->UnregisterFromSense(UAISense_Sight::StaticClass());
	}
	
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
