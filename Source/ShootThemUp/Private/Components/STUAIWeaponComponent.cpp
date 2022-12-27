// Shoot Them Up Game. All Rights Reserved.

#include "Components/STUAIWeaponComponent.h"
#include "Weapons/STUBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogAIWeaponComponent, All, All)
void USTUAIWeaponComponent::StartFire()
{
    if(!CanDoAction()) return;
    
    if(CurrentWeapon->IsAmmoEmpty())
    {
        NextWeapon();
    }
    else
    {
        CurrentWeapon->StartFire();
    }
}

void USTUAIWeaponComponent::NextWeapon()
{
    if(!CanDoAction()) return;
    
    int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    while(NextIndex != CurrentWeaponIndex)
    {
        if(!Weapons[NextIndex]->IsAmmoEmpty()) break;
        NextIndex = (NextIndex + 1) % Weapons.Num();
    }
    if(CurrentWeaponIndex != NextIndex)
    {
        CurrentWeaponIndex = NextIndex;
        EquipWeapon(CurrentWeaponIndex);
    }
}