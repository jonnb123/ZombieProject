// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieGame/Characters/Turret/BTTask_FireAtZombie.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "ZombieGame/Characters/Turret/Turret.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_FireAtZombie::UBTTask_FireAtZombie()
{
    NodeName = TEXT("FireAtZombie");
}

EBTNodeResult::Type UBTTask_FireAtZombie::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    AIController = OwnerComp.GetAIOwner();
    
    const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    AActor* ZombieActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("Zombie"));

    if (const AZombie* Zombie = Cast<AZombie>(ZombieActor))
    {
        APawn* TurretPawn = AIController->GetPawn();
        ATurret* Turret = Cast<ATurret>(TurretPawn);
        const FVector ZombieLocation = Zombie->GetActorLocation();
        const FVector TurretLocation = Turret->GetActorLocation();
        const FRotator TurretRotation = Turret->GetActorRotation();
        const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(TurretLocation, ZombieLocation);

        const FRotator InterpValue = UKismetMathLibrary::RInterpTo(TurretRotation, LookAtRotation, GetWorld()->GetDeltaSeconds(), 100.0);
        // REMEMBER: PITCH (y), YAW (z), ROLL (x)
        Turret->SetActorRotation(FRotator(0, InterpValue.Yaw, 0));
        
        FActorSpawnParameters ActorSpawnParams;
        ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        FVector ProjectileDirection = Turret->BulletSpawn->GetComponentRotation().Vector();
        AZombieGameProjectile *Projectile = GetWorld()->SpawnActor<AZombieGameProjectile>(Turret->ProjectileClass, Turret->BulletSpawn->GetComponentLocation(), Turret->BulletSpawn->GetComponentRotation(), ActorSpawnParams);
    }
    
    return EBTNodeResult::Succeeded;
}
