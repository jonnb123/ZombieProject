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
    AZombieAIController *ZombieAIController = Cast<AZombieAIController>(AIController);
    const APawn *AIPawn{AIController->GetPawn()};
    ACharacter *AICharacter{AIController->GetCharacter()};
    AZombie *ZombieCharacter = Cast<AZombie>(AICharacter);

    // gets the character
    ACharacter *Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    // get grandad
    AGrandad *Grandad = AGrandad::GetInstance();

    // Get front door
    AFrontDoor *FrontDoor = AFrontDoor::GetInstance();

    // Gets the Animation Instance and if it has ended play OnAttackEnd
    UAnimInstance *AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        // Bind a delegate function to the OnMontageEnded event
        AnimInstance->OnMontageEnded.AddDynamic(this, &UBTTask_AttackPlayer::OnAttackEnd);
    }

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

        // This determines whether to damage character or Grandad
        if (IsCharacter == true)
        {
            ZombieAIController->Target = Player;
        }
        else if (IsGrandad == true)
        {
           ZombieAIController->Target = Grandad;
        }
        else if (IsDoor == true)
        {
            ZombieAIController->Target = FrontDoor;
        }

        IDamageableInterface *TheInterface = Cast<IDamageableInterface>(ZombieAIController->Target);
        if (TheInterface)
        {
            TheInterface->HandleDamage(MeleeDamage, DamageEvent, AIController, AICharacter);
        }
    }

    return EBTNodeResult::InProgress;
}
