// Shoot Them Up Game. All Rights Reserved.

#include "UI/STUGameDataWidget.h"
#include "Player/STUPlayerState.h"
#include "ShootThemUp/STUGameModeBase.h"
#include "Kismet/KismetStringLibrary.h"

FString USTUGameDataWidget::GetKillsDeathsText() const
{
    const auto KillsNum = GetKillsNum();
    const auto DeathsNum = GetDeathsNum();
    
    return FString::Printf(TEXT("Kills: %i | Deaths: %i"), KillsNum, DeathsNum);
}

FString USTUGameDataWidget::GetRoundsText() const
{
    const auto CurrentRoundNum = GetCurrentRoundNum();
    const auto TotalRoundsNum = GetTotalRoundsNum();
    
    return FString::Printf(TEXT("Round: %i / %i"), CurrentRoundNum, TotalRoundsNum);
}

FString USTUGameDataWidget::GetRoundCountDownText() const
{
    const auto RoundSecondsRemaining = GetRoundSecondsRemaining();

    return UKismetStringLibrary::TimeSecondsToString(RoundSecondsRemaining).Mid(0, 5);
}

ASTUGameModeBase* USTUGameDataWidget::GetSTUGameMode() const
{
    return GetWorld() ? Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

ASTUPlayerState* USTUGameDataWidget::GetSTUPlayerState() const
{
    return GetOwningPlayer() ? Cast<ASTUPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}

int32 USTUGameDataWidget::GetKillsNum() const
{
    const auto PlayerState = GetSTUPlayerState();
    return PlayerState ? PlayerState->GetKilsNum() : 0;
}

int32 USTUGameDataWidget::GetDeathsNum() const
{
    const auto PlayerState = GetSTUPlayerState();
    return PlayerState ? PlayerState->GetDeathsNum() : 0;
}

int32 USTUGameDataWidget::GetCurrentRoundNum() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}

int32 USTUGameDataWidget::GetTotalRoundsNum() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetGameData().RoundsNum : 0;
}

int32 USTUGameDataWidget::GetRoundSecondsRemaining() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetRoundSecondsRemaining() : 0;
}
