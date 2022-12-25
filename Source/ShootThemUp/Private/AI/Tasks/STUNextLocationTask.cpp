// Shoot Them Up Game. All Rights Reserved.

#include "AI/Tasks/STUNextLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

USTUNextLocationTask::USTUNextLocationTask()
{
    NodeName = "Next Location";
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner(); // get AI controller
    const auto BlackBoard = OwnerComp.GetBlackboardComponent();

    if (!Controller || !BlackBoard)
    {
        return EBTNodeResult::Failed;
    }

    const auto Pawn = Controller->GetPawn();
    if (!Pawn)
    {
        return EBTNodeResult::Failed;
    }
    
    const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavSys)
    {
        return EBTNodeResult::Failed;
    }

    FNavLocation NavLocation;
    // get random point on Nav Mesh where our pawn can go using pathfinding algorithm
    const auto Found = NavSys->GetRandomPointInNavigableRadius(Pawn->GetActorLocation(), Radius, NavLocation);
    if (!Found)
    {
        return EBTNodeResult::Failed;
    }

    // Write location as vector to blackboard
    BlackBoard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
    return EBTNodeResult::Succeeded;
}