// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUPlayerHUDWidget.h"

#include "STUPlayerController.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"
#include "Components/ProgressBar.h"
#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerHUDWidget, All, All)

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    if(!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if(!WeaponComponent) return false;

    return WeaponComponent->GetWeaponUIData(UIData);
}

FString USTUPlayerHUDWidget::GetCurrentBulletsUIText() const
{
    FAmmoData CurrentAmmo;
    if(GetCurrentAmmoData(CurrentAmmo))
    {
        return FormatBullets(CurrentAmmo.Bullets);
    }
    return FString(TEXT("Error"));
}

FString USTUPlayerHUDWidget::GetCurrentClipsUIText() const
{
    FAmmoData CurrentAmmo;
    if(GetCurrentAmmoData(CurrentAmmo))
    {
        if(!CurrentAmmo.bInfinite)
        {
            return FString::FromInt(CurrentAmmo.Clips);
        }
        return FString(TEXT("∞"));
    }
    return FString(TEXT("Error"));
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

void USTUPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
    if(HealthDelta < 0.0f)
    {
        OnTakeDamage();
    }
    UpdateHealthBar();
}

void USTUPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(NewPawn);
    if(HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
    }
    UpdateHealthBar();
}

void USTUPlayerHUDWidget::UpdateHealthBar()
{
    if(HealthProgressBar)
    {
        HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor);
    }
}

void USTUPlayerHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if(GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

bool USTUPlayerHUDWidget::GetCurrentAmmoData(FAmmoData& CurrentAmmo) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
    if(!WeaponComponent) return false;
    return WeaponComponent->GetCurrentAmmoData(CurrentAmmo);
}

FString USTUPlayerHUDWidget::GetKillsDeathsText() const
{
    const auto KillsNum = GetKillsNum();
    const auto DeathsNum = GetDeathsNum();
    
    return FString::Printf(TEXT("%i/%i"), KillsNum, DeathsNum);
}

ASTUPlayerState* USTUPlayerHUDWidget::GetSTUPlayerState() const
{
    return GetOwningPlayer() ? Cast<ASTUPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}

int32 USTUPlayerHUDWidget::GetKillsNum() const
{
    const auto PlayerState = GetSTUPlayerState();
    return PlayerState ? PlayerState->GetKilsNum() : 0;
}

int32 USTUPlayerHUDWidget::GetDeathsNum() const
{
    const auto PlayerState = GetSTUPlayerState();
    return PlayerState ? PlayerState->GetDeathsNum() : 0;
}

FString USTUPlayerHUDWidget::FormatBullets(int32 BulletsNum) const
{
    const int32 MaxLen = 3;
    const TCHAR PrefixSymbol = '0';

    auto BulletStr = FString::FromInt(BulletsNum);
    const auto SymbolsNumtoAdd = MaxLen - BulletStr.Len();

    if(SymbolsNumtoAdd > 0)
    {
        BulletStr = FString::ChrN(SymbolsNumtoAdd, PrefixSymbol).Append(BulletStr);
    }
    return BulletStr;
}
