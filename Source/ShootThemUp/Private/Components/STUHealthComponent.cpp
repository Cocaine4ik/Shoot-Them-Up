// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Dev/STUFireDamageType.h"
#include "Dev/STUIceDamageType.h"

DEFINE_LOG_CATEGORY_STATIC(LogHeatlhComponent, All, All)

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    Health = MaxHealth;

    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
    AActor* DamageCauser)
{
    Health -= Damage;
    UE_LOG(LogHeatlhComponent, Display, TEXT("Damage: %f"), Damage);

    if(DamageType)
    {
        if(DamageType->IsA<USTUFireDamageType>())
        {
            UE_LOG(LogHeatlhComponent, Display, TEXT("So hoooooooot !!"));
        }
        else if (DamageType->IsA<USTUIceDamageType>())
        {
            UE_LOG(LogHeatlhComponent, Display, TEXT("So Colddd..."))
        }
    }
}