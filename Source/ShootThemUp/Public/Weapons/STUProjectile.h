// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "STUProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
    GENERATED_BODY()

public:
    ASTUProjectile();

    void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }
    void SetDamageAmount(const float Damage) { DamageAmount = Damage; }
    void SetDamageRadius(const float Radius) { DamageRadius = Radius; }
    void SetDoFullDamage(const bool DoFullDamage) { bDoFullDamage = DoFullDamage; }

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float LifeSeconds = 5.0f;
    
    virtual void BeginPlay() override;

private:
    FVector ShotDirection;
    float DamageAmount;
    float DamageRadius;
    bool bDoFullDamage;
    
    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent*HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    AController* GetController() const;
};