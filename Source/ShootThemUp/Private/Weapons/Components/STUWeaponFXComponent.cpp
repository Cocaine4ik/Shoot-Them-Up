// Shoot Them Up Game. All Rights Reserved.


#include "Weapons/Components/STUWeaponFXComponent.h"

#include "MeshPassProcessor.h"
#include "NiagaraFunctionLibrary.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactEffects(const FHitResult& Hit)
{
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
}