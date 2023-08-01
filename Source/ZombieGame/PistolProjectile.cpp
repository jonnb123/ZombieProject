// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolProjectile.h"
#include "Zombie.h"

// void AZombieGameProjectile::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
// {
// 	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
// 	{
// 		// Check if the OtherActor is a zombie
// 		if (OtherActor->IsA(AZombie::StaticClass()))
// 		{
// 			// Apply damage to the zombie
// 			UGameplayStatics::ApplyDamage(OtherActor, 20.f, nullptr, this, UDamageType::StaticClass());
// 		}
// 		// // Add impulse and destroy projectile
// 		// if (OtherComp->IsSimulatingPhysics())
// 		// {
// 		//     OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
// 		// }

// 		FVector ShotDirection = -Rotation.Vector();

// 		AActor *HitActor = Hit.GetActor();
// 		UE_LOG(LogTemp, Log, TEXT("actor is %s"), *HitActor->GetName());
// 		// Get hit surface type
// 		TempSurface = UGameplayStatics::GetSurfaceType(Hit);

// 		UE_LOG(LogTemp, Log, TEXT("The surface is %s"), *UEnum::GetValueAsString(TempSurface));
// 		if (HitActor != nullptr)
// 		{
// 			if (TempSurface == SurfaceType1) // for headshots on zombie
// 			{
// 				FPointDamageEvent DamageEvent(Weapons[CurrentWeaponIndex]->HeadDamage, Hit, ShotDirection, nullptr);
// 				HitActor->TakeDamage(Weapons[CurrentWeaponIndex]->HeadDamage, DamageEvent, GetInstigatorController(), this); // this calls the takedamage function in zombie.cpp when the zombie is hit
// 				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HeadshotFX, Hit.Location, ShotDirection.Rotation());
// 				Points += 50;
// 			}
// 			else if (TempSurface == SurfaceType2) // for body shots on zombie
// 			{
// 				UE_LOG(LogTemp, Log, TEXT("Surface 2"));
// 				FPointDamageEvent DamageEvent(Weapons[CurrentWeaponIndex]->BodyDamage, Hit, ShotDirection, nullptr);
// 				HitActor->TakeDamage(Weapons[CurrentWeaponIndex]->BodyDamage, DamageEvent, GetInstigatorController(), this);
// 				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BodyShotFX, Hit.Location, ShotDirection.Rotation());
// 				Points += 10;
// 			}
// 			else if (TempSurface == SurfaceType3) // for the fire boss
// 			{
// 				UE_LOG(LogTemp, Log, TEXT("Entered Fireboss loop"));
// 				FPointDamageEvent DamageEvent(Weapons[CurrentWeaponIndex]->BodyDamage, Hit, ShotDirection, nullptr);
// 				HitActor->TakeDamage(Weapons[CurrentWeaponIndex]->BodyDamage, DamageEvent, GetInstigatorController(), this);
// 				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireImpactEffect, Hit.Location, ShotDirection.Rotation());
// 				Points += 10;
// 			}
// 			else
// 			{
// 				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
// 				UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletHole, FVector(15, 15, 15), Hit.Location, ShotDirection.Rotation());
// 			}
// 		}

// 		Destroy();
// 	}
// }

void APistolProjectile::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    Destroy();
}

APistolProjectile::APistolProjectile()
{
    HeadDamage = 20;
    TorsoDamage = 15; 
    LegDamage = 10;
    ArmDamage = 10;
}
