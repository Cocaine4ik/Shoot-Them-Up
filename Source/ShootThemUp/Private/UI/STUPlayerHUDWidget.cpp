// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerHUDWidget, All, All)

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto Player = GetOwningPlayerPawn();

    if(!Player) return 0.0f;

    const auto Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());
    const auto HealthComponent = Cast<USTUHealthComponent>(Component);
    if(!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = GetWeaponComponent();
    if(!WeaponComponent) return false;

    return WeaponComponent->GetWeaponUIData(UIData);
}

FString USTUPlayerHUDWidget::GetCurrentAmmoUIText() const
{
    FAmmoData CurrentAmmo;
    if(GetCurrentAmmoData(CurrentAmmo))
    {
        if(!CurrentAmmo.bInfinite)
        {
            return FString::FromInt(CurrentAmmo.Bullets) + FString(TEXT(" / ")) + FString::FromInt(CurrentAmmo.Clips);
        }
        return FString::FromInt(CurrentAmmo.Bullets) + FString(TEXT(" / ∞"));
    }
    return FString(TEXT("Error / Error"));
}

USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
{
    const auto Player = GetOwningPlayerPawn();

    if(!Player) return nullptr;

    const auto Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<USTUWeaponComponent>(Component);
    return WeaponComponent;
}

bool USTUPlayerHUDWidget::GetCurrentAmmoData(FAmmoData& CurrentAmmo) const
{
    const auto WeaponComponent = GetWeaponComponent();
    if(!WeaponComponent) return false;
    return WeaponComponent->GetCurrentAmmoData(CurrentAmmo);
}