// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWeapon.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUWeaponComponent();

    virtual void StartFire();
    void StopFire();
    virtual void NextWeapon();
    void Reload();
    bool IsFiring() const;

    bool GetWeaponUIData(FWeaponUIData& UIData)const;
    bool GetCurrentAmmoData(FAmmoData& CurrentAmmo) const;

    bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount);
    bool NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType);

    void Zoom(bool bEnabled);
    
protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    int32 CurrentWeaponIndex = 0;
    
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
    bool CanDoAction() const;
    void EquipWeapon(int32 WeaponIndex);
private:

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;
    
    bool bEquipAnimInProgress = false;
    bool bReloadAnimInProgress = false;
    
    void SpawnWeapons();

    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

    void PlayAnimMontage(UAnimMontage* Animation);
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnWeaponChange(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);
    
    bool CanReload() const;

    void OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon);
    void ChangeClip();
};