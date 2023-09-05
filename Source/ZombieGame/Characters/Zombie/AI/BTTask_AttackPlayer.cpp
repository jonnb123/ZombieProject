// Fill out your copyright notice in the Description page of Project Settings.
#include "ZombieGame/Characters/Zombie/AI/BTTask_AttackPlayer.h"
#include "AIController.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "AttackNotify.h"
#include "Kismet/GameplayStatics.h"

UBTTask_AttackPlayer::UBTTask_AttackPlayer()
{
    NodeName = TEXT("Attack player");
   
}

EBTNodeResult::Type UBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    FinishedAttack = false;

    // this block gets the Zombie
    AAIController *AIController{OwnerComp.GetAIOwner()};
    const APawn *AIPawn{AIController->GetPawn()};
    ACharacter *AICharacter{AIController->GetCharacter()};
    AZombie *ZombieCharacter = Cast<AZombie>(AICharacter);

    // gets the character
    ACharacter *Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    UE_LOG(LogTemp, Log, TEXT("Damaging..."));
    UE_LOG(LogTemp, Log, TEXT("Name of actor: %s"), *AICharacter->GetName());

    FDamageEvent DamageEvent;

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
        OnAttackEnd.AddDynamic(this, &UBTTask_AttackPlayer::AttackFinished);

        // Player->TakeDamage(MeleeDamage, DamageEvent, AIController, AICharacter); // this is where the TakeDamage function from the ZombieGameCharacter is called.
    }
    return EBTNodeResult::Succeeded;
}

void UBTTask_AttackPlayer::AttackFinished()
{
    UE_LOG(LogTemp, Warning, TEXT("HELLO FINISHED NOW"));
}
