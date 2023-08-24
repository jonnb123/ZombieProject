// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation() 
{
    NodeName = TEXT("Find Random Location");

    // accept only vectors 
    BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomLocation, BlackboardKey));
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) 
{
    FNavLocation Location{};

    // get ai pawn. the {} are initaliser lists, to initialise aicontroller and aipawn
    AAIController* AIController {OwnerComp.GetAIOwner()};
    const APawn* AIPawn {AIController->GetPawn()};

    // get pawn origin
    const FVector Origin {AIPawn->GetActorLocation()};

    // obtain navigation system and find random location
    const UNavigationSystemV1* NavSystem {UNavigationSystemV1::GetCurrent(GetWorld())};
    if (IsValid(NavSystem) && NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
    {
        AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
    }

    // signal the behavior tree component that the task finished with a success
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}

FString UBTTask_FindRandomLocation::GetStaticDescription() const 
{
    return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}




