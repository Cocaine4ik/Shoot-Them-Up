// Shoot Them Up Game. All Rights Reserved.


#include "Weapons/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "STUBaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)
// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::StartFire()
{

}

void ASTUBaseWeapon::StopFire()
{

}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    
    check(WeaponMesh)
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count coudn't be less or equal zero"))
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips count coudn't be less or equal zero"))
    
    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::MakeShot()
{
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if(!Player) return nullptr;

    return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto Controller = GetPlayerController();
    if(!Controller) return false;
    
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;
    
    TraceStart = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if(!GetWorld()) return;
    
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    if(CurrentAmmo.Bullets == 0)
    {
        UE_LOG(LogBaseWeapon, Warning, TEXT("Clip is empty!"));
        return;
    }
    CurrentAmmo.Bullets--;
    LogAmmo();

    if(IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast();
    }
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.bInfinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

void ASTUBaseWeapon::ChangeClip()
{
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    if(!CurrentAmmo.bInfinite)
    {
        if(CurrentAmmo.Clips == 0)
        {
            UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips!"));
            return;
        }
        CurrentAmmo.Clips--;
    }
    UE_LOG(LogBaseWeapon, Display, TEXT("....Change Clip...."));
}

bool ASTUBaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

void ASTUBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
    AmmoInfo += CurrentAmmo.bInfinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}