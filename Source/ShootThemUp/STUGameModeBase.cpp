// Shoot Them Up Game. All Rights Reserved.

#include "STUGameModeBase.h"

#include "AIController.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"
#include "Player/STUPlayerState.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"
#include "EngineUtils.h"
#include "STUPlayerStart.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All)

constexpr static int32 MinRoundTimeForRespawn = 10;

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    checkf(GetPlayerStartsCount() == GameData.PlayersNum, TEXT("Player Starts Count is not equal Players Num"))
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    CreateTeamsInfo();
    
    CurrentRound = 1;
    StartRound();
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if(InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

AActor* ASTUGameModeBase::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
    UWorld* World = GetWorld();
    
    const auto PlayerState = Player->GetPlayerState<ASTUPlayerState>();
    
    if (PlayerState)
    {    
        for (TActorIterator<ASTUPlayerStart> It(World); It; ++It)
        {
            ASTUPlayerStart* Start = *It;
            if (Start && Start->GetTeamID() == PlayerState->GetTeamID())
            {
                return Start;
            }
        }
    }

    // Always pick StartSpot at start of match
    if (ShouldSpawnAtStartSpot(Player))
    {
        if (AActor* PlayerStartSpot = Player->StartSpot.Get())
        {
            return PlayerStartSpot;
        }
        else
        {
            UE_LOG(LogGameMode, Error, TEXT("FindPlayerStart: ShouldSpawnAtStartSpot returned true but the Player StartSpot was null."));
        }
    }
    AActor* BestStart = ChoosePlayerStart(Player);
    if (BestStart == nullptr)
    {
        // No player start found
        UE_LOG(LogGameMode, Log, TEXT("FindPlayerStart: PATHS NOT DEFINED or NO PLAYERSTART with positive rating"));

        // This is a bit odd, but there was a complex chunk of code that in the end always resulted in this, so we may as well just 
        // short cut it down to this.  Basically we are saying spawn at 0,0,0 if we didn't find a proper player start
        BestStart = World->GetWorldSettings();
    }

    return BestStart;
}

void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
    const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
    const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

    if(KillerPlayerState)
    {
        KillerPlayerState->AddKill();
    }
    if(VictimPlayerState)
    {
        VictimPlayerState->AddDeath();
    }
    StartRespawn(VictimController);
}

void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

bool ASTUGameModeBase::IsRespawnAvailable() const
{
    return RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
}

void ASTUGameModeBase::StartRespawn(AController* Controller)
{
    const auto bRespawnAvailable = IsRespawnAvailable();
    if(!bRespawnAvailable) return;
    
    const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller);
    if(!RespawnComponent) return;

    RespawnComponent->Respawn(GameData.RespawnTime);
}

void ASTUGameModeBase::GameOver()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("====== GAME OVER ======"));
    LogPlayerInfo();

    for(auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if(Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }
}

int32 ASTUGameModeBase::GetPlayerStartsCount()
{
    int32 Count = 0;
        
    if(!GetWorld()) return Count;
    for (TActorIterator<ASTUPlayerStart> It(GetWorld()); It; ++It)
    {
        Count++;
    }
    return Count;
}

void ASTUGameModeBase::LogPlayerInfo()
{
    if(!GetWorld()) return;
    
    for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if(!Controller) continue;

        const auto PlayerSate = Cast<ASTUPlayerState>(Controller->PlayerState);
        if(!PlayerSate) continue;

        PlayerSate->LogInfo();
    }
}

void ASTUGameModeBase::SpawnBots()
{
    if(!GetWorld()) return;

    for(int32 i = 0; i < GameData.PlayersNum - 1; i++)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(STUAIController);
    }
}

void ASTUGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundsTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("Timer: %i / Round: %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);
    
    if(--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        if(CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {
            GameOver();
        }
    }
}

void ASTUGameModeBase::ResetPlayers()
{
    if(!GetWorld()) return;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
    if(Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void ASTUGameModeBase::CreateTeamsInfo()
{
    if(!GetWorld()) return;

    int32 TeamID = 1;
    for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if(!Controller) continue;

        const auto PlayerSate = Cast<ASTUPlayerState>(Controller->PlayerState);
        if(!PlayerSate) continue;

        PlayerSate->SetTeamID(TeamID);
        PlayerSate->SetTeamColor(DetermineColorByTeamID(TeamID));
        SetPlayerColor(Controller);
        
        TeamID = TeamID == 1 ? 2 : 1;
    }
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID)
{
    if(TeamID -1 < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamID -1];
    }
    UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for teeam id: &i, set to default: %s"), TeamID, *GameData.DefaultTeamColor.ToString());
    return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
    if(!Controller) return;

    const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
    if(!Character) return;

    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    Character->SetPlayerColor(PlayerState->GetTeamColor());
    
}