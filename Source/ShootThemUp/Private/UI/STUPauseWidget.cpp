// Shoot Them Up Game. All Rights Reserved.

#include "UI/STUPauseWidget.h"

#include "Animation/AnimSubsystem_NodeRelevancy.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"
#include "STUGameInstance.h"
#include "Logging/LogMacros.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUPauseWidget, All, All)

void USTUPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if(ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
    }
    if(MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnMainMenu);
    }
    
}

void USTUPauseWidget::OnClearPause()
{
    if(!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

    GetWorld()->GetAuthGameMode()->ClearPause();
    
}

void USTUPauseWidget::OnMainMenu()
{
    if(!GetWorld()) return;

    const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if(!STUGameInstance) return;

    if(STUGameInstance->GetMainMenuLevelName().IsNone())
    {
        UE_LOG(LogSTUPauseWidget, Error, TEXT("Level name is NONE"));
        return;
    }
    
    UGameplayStatics::OpenLevel(this, STUGameInstance->GetMainMenuLevelName());
}