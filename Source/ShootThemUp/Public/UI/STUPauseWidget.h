// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUPauseWidget.generated.h"

class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUPauseWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ClearPauseButton;

    UPROPERTY(meta = (BindWidget))
    UButton* MainMenuButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnClearPause();

    UFUNCTION()
    void OnMainMenu();
};