// Shoot Them Up Game. All Rights Reserved.

#include "UI/STUSpectatorWidget.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"

bool USTUSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
    const auto PawnName = GetOwningPlayer()->GetName();
    
    const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(GetOwningPlayer());

    if(!RespawnComponent) return false;
    
    CountDownTime = RespawnComponent->GetRespawnCountDown();
    return true;
}