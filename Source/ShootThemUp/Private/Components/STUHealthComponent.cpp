// Shoot Them Up Game. All Rights Reserved.


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
    if(IsHealthFull()) return false;
    
    SetHealth(Health + HealthAmount);
    UE_LOG(LogHealthComponent, Display, TEXT("Health points were restored"));
    return true;
}

bool USTUHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0)
    
    SetHealth(MaxHealth);

    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
    AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld()) return;
    
    SetHealth(Health - Damage);

    GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
    
    if(IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (bAutoHeal && GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(AutoHealTimerHandle, this, &USTUHealthComponent::AutoHealUpdate, HealUpdateTime, true, HealDelay);
    }
    // UE_LOG(LogHealthComponent, Display, TEXT("Damage: %f"), Damage);
}

void USTUHealthComponent::SetHealth(const float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}

void USTUHealthComponent::AutoHealUpdate()
{
    SetHealth(Health + HealModifier);

    if(IsHealthFull() && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
    } 
}