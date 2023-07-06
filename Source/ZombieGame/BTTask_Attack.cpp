// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Zombie.h"
#include "NavigationSystem.h"
#include "Kismet\KismetSystemLibrary.h"
#include "ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"



UBTTask_Attack::UBTTask_Attack() 
{
    NodeName = TEXT("Attack player");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);


    AAIController* AIController {OwnerComp.GetAIOwner()};
    const APawn* AIPawn {AIController->GetPawn()};
    ACharacter* AICharacter {AIController->GetCharacter()};

    // Set what actors to seek out from it's collision channel
    TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
    traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

    // Ignore any specific actors
    TArray<AActor*> ignoreActors;

    // Ignore self or remove this line to not ignore any
    ignoreActors.Init(AICharacter, 1);

    // Array of actors that are inside the radius of the sphere
    TArray<AActor*> outActors;
        
    // Total radius of the sphere
    float radius = 100.0f;
    // Sphere's spawn loccation within the world
    FVector sphereSpawnLocation = AICharacter->GetActorLocation();
    // Class that the sphere should hit against and include in the outActors array (Can be null)
    UClass* seekClass = AZombieGameCharacter::StaticClass(); // NULL;

    bool bSuccess = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), sphereSpawnLocation, radius, traceObjectTypes, seekClass, ignoreActors, outActors);

    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    

    // Optional: Use to have a visual representation of the SphereOverlapActors
    // DrawDebugSphere(GetWorld(), AICharacter->GetActorLocation(), radius, 12, FColor::Red, true, 10.0f);


    if (bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("Damaging..."));
        UE_LOG(LogTemp, Log, TEXT("Name of actor: %s"), *AICharacter->GetName());
        AICharacter->PlayAnimMontage(AttackMontage);
		FDamageEvent DamageEvent;
		
        if (AICharacter->GetName().StartsWith("BP_FireZombie"))
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireZombieAttackSound, AICharacter->GetActorLocation());
            Player->TakeDamage(FireMeleeDamage, DamageEvent, AIController, AICharacter);  

        }
        else
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), ZombieAttackSound, AICharacter->GetActorLocation());
            Player->TakeDamage(MeleeDamage, DamageEvent, AIController, AICharacter);  // this is where the TakeDamage function from the ZombieGameCharacter is called.
        }
       
    }

    
    return EBTNodeResult::Succeeded;
}
