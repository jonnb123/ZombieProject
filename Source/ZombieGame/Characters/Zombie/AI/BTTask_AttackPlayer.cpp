// Fill out your copyright notice in the Description page of Project Settings.
#include "ZombieGame/Characters/Zombie/AI/BTTask_AttackPlayer.h"
#include "AIController.h"
#include "ZombieGame/Characters/Zombie/AI/ZombieAIController.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ZombieGame/Characters/Grandad/Grandad.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieGame/GameMode/ZombieGameMode.h"
#include "ZombieGame/Characters/Zombie/AI/DamageableInterface.h"
#include "ZombieGame/Widgets/ShopItem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


UBTTask_AttackPlayer::UBTTask_AttackPlayer()
{
    NodeName = TEXT("Attack");
}

void UBTTask_AttackPlayer::OnAttackEnd(UAnimMontage *Montage, bool bInterrupted)
{
    GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Blue, "Attack Ended!!");
    FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    // Stores a reference for OwnerComp for use in OnAttackEnd
    CachedOwnerComp = &OwnerComp;

    // this block gets the Zombie
    AAIController *AIController{OwnerComp.GetAIOwner()};
    const AZombieAIController *ZombieAIController = Cast<AZombieAIController>(AIController);
    
    ACharacter *AICharacter{AIController->GetCharacter()};
    const AZombie *ZombieCharacter = Cast<AZombie>(AICharacter);

    UE_LOG(LogTemp, Log, TEXT("Damaging Actor: %s"), *AICharacter->GetName());

    // make sure zombie is dead before attacking.
    if (ZombieCharacter->GetIsZombieDead()) return EBTNodeResult::Aborted;

    // Gets the Animation Instance and if it has ended play OnAttackEnd
    if (UAnimInstance *AnimInstance = AICharacter->GetMesh()->GetAnimInstance())
    {
        AnimInstance->OnMontageEnded.AddDynamic(this, &UBTTask_AttackPlayer::OnAttackEnd);
    }
    
    IDamageableInterface *TheInterface = Cast<IDamageableInterface>(ZombieAIController->Target);
    
    if (AICharacter->GetName().StartsWith("BP_FireZombie"))
    {
        AICharacter->PlayAnimMontage(AttackMontage);
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireZombieAttackSound, AICharacter->GetActorLocation());

        if (TheInterface)
        {
            TheInterface->HandleDamage(FireMeleeDamage, DamageEvent, AIController, AICharacter);
        }
    }
    else // if normal zombie
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

        if (TheInterface)
        {
            UE_LOG(LogTemp, Warning, TEXT("Target: %s"), *ZombieAIController->Target->GetName());
            TheInterface->HandleDamage(MeleeDamage, DamageEvent, AIController, AICharacter);
        }

        
    }

    return EBTNodeResult::InProgress;
}
