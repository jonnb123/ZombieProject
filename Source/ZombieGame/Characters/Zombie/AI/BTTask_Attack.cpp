// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "AIController.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTTask_Attack::UBTTask_Attack()
{
    NodeName = TEXT("Attack player");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    // Set what actors to seek out from its collision channel
    TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
    traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

    // Ignore self
    TArray<AActor *> ignoreActors;
    ignoreActors.Init(AICharacter, 1);

    // Array of actors that are inside the radius of the sphere
    TArray<AActor *> outActors;

    // Total radius of the sphere
    float radius = 100.0f;

    // Sphere's spawn loccation within the world
    FVector sphereSpawnLocation = AICharacter->GetActorLocation();

    // Class that the sphere should hit against and include in the outActors array (Can be null)
    UClass *seekClass = AZombieGameCharacter::StaticClass(); // NULL;

    bool bSuccess = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), sphereSpawnLocation, radius, traceObjectTypes, seekClass, ignoreActors, outActors);

    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Damaging..."));
        UE_LOG(LogTemp, Log, TEXT("Name of actor: %s"), *AICharacter->GetName());

        FDamageEvent DamageEvent;

        // gets the PlayerCharacter
        ACharacter *Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

        // this block gets the Zombie
        AAIController *AIController{OwnerComp.GetAIOwner()};
        const APawn *AIPawn{AIController->GetPawn()};
        ACharacter *AICharacter{AIController->GetCharacter()};
        AZombie *ZombieCharacter = Cast<AZombie>(AICharacter);

        if (AICharacter->GetName().StartsWith("BP_FireZombie"))
        {
            AICharacter->PlayAnimMontage(AttackMontage);
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireZombieAttackSound, AICharacter->GetActorLocation());
            Player->TakeDamage(FireMeleeDamage, DamageEvent, AIController, AICharacter);
        }
        else
        {
            if (ZombieCharacter->GetIsCrawling() == false)
            {
                AICharacter->PlayAnimMontage(AttackMontage);
            }
            else
            {
                AICharacter->PlayAnimMontage(CrawlAttackMontage);
            }
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), ZombieAttackSound, AICharacter->GetActorLocation());
            Player->TakeDamage(MeleeDamage, DamageEvent, AIController, AICharacter); // this is where the TakeDamage function from the ZombieGameCharacter is called.
        }
    }
    return EBTNodeResult::Succeeded;
}
