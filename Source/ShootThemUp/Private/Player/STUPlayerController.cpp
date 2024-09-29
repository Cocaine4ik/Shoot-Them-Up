// Shoot Them Up Game. All Rights Reserved.


#include "Player/STUPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "STUGameHUD.h"
#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"
#include "STUGameInstance.h"
#include "STUPlayerCharacter.h"

ASTUPlayerController::ASTUPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("STURespawnComponent");
}

void ASTUPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
        GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(MetaMappingContext, 1);
    }

    if (GetWorld())
    {
        if (const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ASTUPlayerController::OnMatchStateChanged);
        }
    }
}

void ASTUPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    check(InputComponent)

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(PauseGameAction, ETriggerEvent::Triggered, this, &ASTUPlayerController::OnPauseGame);
        EnhancedInputComponent->BindAction(MuteAction, ETriggerEvent::Triggered, this, &ASTUPlayerController::OnMuteSound);
    }
}

void ASTUPlayerController::OnPauseGame(const FInputActionValue& Value)
{
    if(!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->SetPause(this);
}

void ASTUPlayerController::OnMatchStateChanged(ESTUMatchState State)
{
    if(State == ESTUMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}

void ASTUPlayerController::OnMuteSound(const FInputActionValue& Value)
{
    if(!GetWorld()) return;
    const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if(!STUGameInstance) return;

    STUGameInstance->ToggleVolume();
    
}