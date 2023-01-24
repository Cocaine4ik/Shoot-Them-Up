// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "UI/STUBaseWidget.h"
#include "STUPlayerHUDWidget.generated.h"

class UProgressBar;
class ASTUPlayerState;

UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public USTUBaseWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetWeaponUIData(FWeaponUIData& UIData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    FString GetCurrentBulletsUIText() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    FString GetCurrentClipsUIText() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerAlive() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerSpectating() const;
    
    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void OnTakeDamage();

    UFUNCTION(BlueprintCallable, Category = "UI")
    FString GetKillsDeathsText() const;

protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthProgressBar;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* DamageAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float PercentColorThreshold  = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor GoodColor  = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor BadColor  = FLinearColor::Red;
    
    virtual void NativeOnInitialized() override;
    
private:
    bool GetCurrentAmmoData(FAmmoData& CurrentAmmo) const;

    void OnHealthChanged(float Health, float HealthDelta);

    void OnNewPawn(APawn* NewPawn);

    void UpdateHealthBar();
    
    ASTUPlayerState* GetSTUPlayerState() const;
    int32 GetKillsNum() const;
    int32 GetDeathsNum() const;

    FString FormatBullets(int32 BulletsNum) const;
};