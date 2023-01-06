// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUGameDataWidget.generated.h"

class ASTUPlayerState;
class ASTUGameModeBase;
UCLASS()
class SHOOTTHEMUP_API USTUGameDataWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    FString GetKillsDeathsText() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    FString GetRoundsText() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    FString GetRoundCountDownText() const;
private:
    ASTUGameModeBase* GetSTUGameMode() const;
    ASTUPlayerState* GetSTUPlayerState() const;

    int32 GetKillsNum() const;
    int32 GetDeathsNum() const;
    int32 GetCurrentRoundNum() const;
    int32 GetTotalRoundsNum() const;
    int32 GetRoundSecondsRemaining() const;
};