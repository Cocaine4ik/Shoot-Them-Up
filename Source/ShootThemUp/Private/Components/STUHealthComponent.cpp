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

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    SetHealth(MaxHealth);
    
    OnHealthChanged.Broadcast(Health);

    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
    AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead() && !GetWorld()) return;
    
    SetHealth(Health - Damage);
    OnHealthChanged.Broadcast(Health);

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
}

void USTUHealthComponent::AutoHealUpdate()
{
    SetHealth(Health + HealModifier);
    OnHealthChanged.Broadcast(Health);

    if(Health == MaxHealth && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
    } 
}