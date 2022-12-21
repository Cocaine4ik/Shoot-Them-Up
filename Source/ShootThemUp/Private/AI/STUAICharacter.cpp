// Shoot Them Up Game. All Rights Reserved.


#include "AI/STUAICharacter.h"

#include "STUAIController.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ASTUAIController::StaticClass();
}