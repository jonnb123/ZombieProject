// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieGameProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
// #include "Zombie.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"

AZombieGameProjectile::AZombieGameProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	// CollisionComp->InitSphereRadius(5.0f);
	CollisionComponent->InitBoxExtent(FVector(4.5f, 4.5f, 4.5f));
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AZombieGameProjectile::OnHit); // set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComponent;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;


	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AZombieGameProjectile::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	// ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	// AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	// Destroy();
}
