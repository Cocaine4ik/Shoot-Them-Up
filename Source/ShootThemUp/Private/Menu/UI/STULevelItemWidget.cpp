// Shoot Them Up Game. All Rights Reserved.

#include "Menu/UI/STULevelItemWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTULevelItemWidget::SetLevelData(const FLevelData& Data)
{
    LevelData = Data;

    if(LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromName(LevelData.LevelName));
    }

    if(LevelImage)
    {
        LevelImage->SetBrushFromTexture(LevelData.LevelThumb);
    }
}

void USTULevelItemWidget::SetSelected(bool bIsSelected)
{
    if(FrameImage)
    {
        FrameImage->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void USTULevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    
    if(LevelSelectButton)
    {
        LevelSelectButton->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnLevelItemClicked);
    }
}

void USTULevelItemWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(LevelData);
}