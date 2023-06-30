// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "ZombieGameProjectile.h"
#include "Kismet/KismetMathLibrary.h"



// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	RootComponent = TurretMesh;

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(RootComponent);

	FireRate = 2.0f;
	FireRange = 1000.f;
	Health = 100.f;

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombie::StaticClass(), Zombies);

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurret::CheckFireCondition, FireRate, true);
}

void ATurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Zombies.Num() > 0)
    {
        AZombie* ClosestZombie = GetClosestZombie();
        if (ClosestZombie)
        {
            FVector LookAtTarget = ClosestZombie->GetActorLocation();
            LookAtTarget.Z = GunMesh->GetComponentLocation().Z;

            FRotator TargetRotation = (LookAtTarget - GunMesh->GetComponentLocation()).Rotation();

            float RotationSpeed = 1000.f; // adjust this to change the speed of rotation

            // Use RInterpConstantTo to interpolate smoothly between the current rotation and the target rotation
            FRotator NewRotation = FMath::RInterpConstantTo(GunMesh->GetComponentRotation(), TargetRotation, DeltaTime, RotationSpeed);

            GunMesh->SetWorldRotation(NewRotation);
        }
    }
}


AZombie* ATurret::GetClosestZombie()
{
    AZombie* ClosestZombie = nullptr;
    float MinDistance = MAX_FLT;
    FVector TurretLocation = GetActorLocation();

    for (AActor* Actor : Zombies)
    {
        AZombie* Zombie = Cast<AZombie>(Actor);
        if (Zombie && !Zombie->IsDead)
        {
            float Distance = FVector::Dist(Actor->GetActorLocation(), TurretLocation);
            if (Distance < MinDistance)
            {
                ClosestZombie = Zombie;
                MinDistance = Distance;
            }
        }
    }

    return ClosestZombie;
}


void ATurret::CheckFireCondition()
{
	if (Zombies.Num() == 0)
	{
		return;
	}

	AZombie* ClosestZombie = GetClosestZombie();
	if (ClosestZombie && !ClosestZombie->IsDead)
	{
		float Distance = FVector::Dist(ClosestZombie->GetActorLocation(), GetActorLocation());
		if (Distance <= FireRange)
		{
			Fire();
		}
	}
}


void ATurret::Fire()
{
    AActor* CurrentTarget = GetClosestZombie();
    if (!CurrentTarget)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    FVector Location;
    FRotator Rotation;

    FVector StartLocation = GunMesh->GetSocketLocation("Muzzle") + 100.f * GunMesh->GetForwardVector();
    FVector EndLocation = CurrentTarget->GetActorLocation();


    FCollisionQueryParams TraceParams = FCollisionQueryParams::DefaultQueryParam;
    TraceParams.bReturnPhysicalMaterial = true;
    TraceParams.AddIgnoredActor(this);

    FVector MuzzleLocation = StartLocation;
    FRotator MuzzleRotation = GunMesh->GetSocketRotation("Muzzle");


    FVector ProjectileDirection = MuzzleRotation.Vector();

    FRotator ProjectileRotation = ProjectileDirection.Rotation();


    //Set Spawn Collision Handling Override
    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AZombieGameProjectile* Projectile = World->SpawnActor<AZombieGameProjectile>(ProjectileBlueprint, MuzzleLocation, ProjectileRotation, ActorSpawnParams);
    Projectile->GetProjectileMovement()->InitialSpeed = ProjectileDirection.Size() * Projectile->GetProjectileMovement()->MaxSpeed;
    Projectile->GetProjectileMovement()->Velocity = ProjectileDirection * Projectile->GetProjectileMovement()->InitialSpeed;
    Projectile->GetProjectileMovement()->Activate();
    

    // bool bHit = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_WorldDynamic, TraceParams);
    // if (bHit)
    // {
    //     DrawDebugLine(GetWorld(), StartLocation, HitResult.Location, FColor::Red, false, 2.f);
    //     AActor* HitActor = HitResult.GetActor();
    //     UE_LOG(LogTemp, Log, TEXT("actor is %s"), *HitActor->GetName());

    //     if (HitActor == CurrentTarget) // only apply damage if the hit actor is the current target
    //     {
    //         UGameplayStatics::ApplyDamage(CurrentTarget, 20.f, nullptr, this, UDamageType::StaticClass());
    //     }
    // }
    // else
    // {
    //     DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 2.f);
    // }
}



// void ATurret::Fire()
// {
// 	AActor* CurrentTarget = GetClosestZombie();
// 	if (!CurrentTarget)
// 	{
// 		return;
// 	}

// 	UWorld* World = GetWorld();
// 	if (!World)
// 	{
// 		return;
// 	}

// 	FVector Location;
// 	FRotator Rotation; 

// 	FVector StartLocation = GunMesh->GetComponentLocation();
// 	FVector EndLocation = CurrentTarget->GetActorLocation() - (CurrentTarget->GetActorForwardVector() * MaxBulletRange);
// 	// FVector EndLocation = CurrentTarget->GetActorLocation();

// 	FCollisionQueryParams TraceParams = FCollisionQueryParams::DefaultQueryParam;
// 	TraceParams.bReturnPhysicalMaterial = true;
// 	TraceParams.AddIgnoredActor(this);

// 	FVector MuzzleLocation = StartLocation;
//     FRotator MuzzleRotation = GunMesh->GetComponentRotation();

// 	// FVector MuzzleLocation = GunMesh->GetSocketLocation("Muzzle");
// 	// FRotator MuzzleRotation = GunMesh->GetSocketRotation("Muzzle");

// 	// Calculate the direction vector between the muzzle location and the target location
//     FVector ProjectileDirection = (EndLocation - StartLocation).GetSafeNormal();
// 	ProjectileDirection = ProjectileDirection.RotateAngleAxis(GunMesh->GetComponentRotation().Yaw, FVector(0.f, 0.f, 1.f));
// 	FRotator ProjectileRotation = ProjectileDirection.Rotation();

// 	// Rotate the direction vector by the same amount as the turret's rotation
// 	// Direction = Direction.RotateAngleAxis(GunMesh->GetComponentRotation().Yaw, FVector(0.f, 0.f, 1.f));

//     // Use the direction vector to calculate the rotation for the projectile
//     // FRotator ProjectileRotation = Direction.Rotation();

// 	// FVector ProjectileDirection = (CurrentTarget->GetActorLocation() - MuzzleLocation).GetSafeNormal();
// 	// ProjectileDirection = ProjectileDirection.RotateAngleAxis(GunMesh->GetComponentRotation().Yaw, FVector(0.f, 0.f, 1.f));

// 	// FRotator ProjectileRotation = ProjectileDirection.Rotation();

// 	//Set Spawn Collision Handling Override
// 	FActorSpawnParameters ActorSpawnParams;
// 	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

// 	AZombieGameProjectile* Projectile = World->SpawnActor<AZombieGameProjectile>(ProjectileBlueprint, MuzzleLocation, ProjectileRotation, ActorSpawnParams);
// 	Projectile->GetProjectileMovement()->SetVelocityInLocalSpace(ProjectileDirection * Projectile->GetProjectileMovement()->InitialSpeed); 

// 	// bool bHit = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_WorldDynamic, TraceParams);
// 	// if (bHit)
// 	// {
// 	// 	DrawDebugLine(GetWorld(), StartLocation, HitResult.Location, FColor::Red, false, 2.f);
// 	// 	AActor* HitActor = HitResult.GetActor();
// 	// 	UE_LOG(LogTemp, Log, TEXT("actor is %s"), *HitActor->GetName());

		

		
//     //     if (HitActor != nullptr) // only apply damage if the hit actor is the current target
//     //     {
// 	// 		UE_LOG(LogTemp, Warning, TEXT("WE GOT HIM"));
//     //         UGameplayStatics::ApplyDamage(CurrentTarget, 20.f, nullptr, this, UDamageType::StaticClass());
//     //     }
// 	// }
// 	// else
// 	// {
// 	// 	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 2.f);
// 	// }
// }


// void ATurret::Fire()
// {
//     AActor* CurrentTarget = GetClosestZombie();
//     if (!CurrentTarget)
//     {
//         return;
//     }

//     UWorld* World = GetWorld();
//     if (!World)
//     {
//         return;
//     }

//     FVector Location;
//     FRotator Rotation; 

//     FVector StartLocation = GunMesh->GetComponentLocation();
//     FVector EndLocation = CurrentTarget->GetActorLocation() - (CurrentTarget->GetActorForwardVector() * MaxBulletRange);

//     FCollisionQueryParams TraceParams = FCollisionQueryParams::DefaultQueryParam;
//     TraceParams.bReturnPhysicalMaterial = true;
//     TraceParams.AddIgnoredActor(this);

//     bool bHit = World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_WorldDynamic, TraceParams);
//     if (bHit)
//     {
//         DrawDebugLine(GetWorld(), StartLocation, HitResult.Location, FColor::Red, false, 2.f);
//         AActor* HitActor = HitResult.GetActor();
//         UE_LOG(LogTemp, Log, TEXT("actor is %s"), *HitActor->GetName());
        
//         if (HitActor != nullptr) // only apply damage if the hit actor is the current target
//         {
//             UE_LOG(LogTemp, Warning, TEXT("WE GOT HIM"));

//             // Spawn a projectile instance at the muzzle location of the turret
//             FVector MuzzleLocation = StartLocation;
//             FRotator MuzzleRotation = GunMesh->GetComponentRotation();
//             AZombieGameProjectile* Projectile = World->SpawnActor<AZombieGameProjectile>(ProjectileBlueprint, MuzzleLocation, MuzzleRotation);
//             // Set the projectile's velocity towards the target
//             UProjectileMovementComponent* ProjectileMovement = Projectile->FindComponentByClass<UProjectileMovementComponent>();
// 			FVector ProjectileVelocity = (EndLocation - MuzzleLocation).GetSafeNormal() * ProjectileMovement->InitialSpeed
//             ProjectileMovement->SetVelocityInLocalSpace(ProjectileVelocity);

//             // Set the projectile's damage
//             UDamageType* DamageType = UDamageType::StaticClass()->GetDefaultObject<UDamageType>();
//         }
//     }
//     else
//     {
//         DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 2.f);
//     }
// }