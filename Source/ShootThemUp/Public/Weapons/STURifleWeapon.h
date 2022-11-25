// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    float TimeBetweenShorts = 0.1f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread = 1.5f;
    
    virtual void MakeShot() override;
    
private:
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
    FTimerHandle ShotTimerHandle;
};