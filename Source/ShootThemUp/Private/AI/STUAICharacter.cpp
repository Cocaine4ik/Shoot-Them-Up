// Shoot Them Up Game. All Rights Reserved.


#include "AI/STUAICharacter.h"
#include "STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;

    auto MovementComponent = GetCharacterMovement();
    if(MovementComponent)
    {
        MovementComponent->bUseControllerDesiredRotation = true;
        MovementComponent->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
}

void ASTUAICharacter::OnDeath()
{
    Super::OnDeath();
    const auto AIController = Cast<AAIController>(Controller);
    if(AIController && AIController->BrainComponent)
    {
        AIController->BrainComponent->Cleanup();
    }
}