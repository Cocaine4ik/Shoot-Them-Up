// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/STUWeaponComponent.h"

#include "STUBaseCharacter.h"
#include "Weapons/STUBaseWeapon.h"
#include "GameFramework/Character.h"

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::Fire()
{
    if(!CurrentWeapon) return;
    CurrentWeapon->Fire();
}

void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

    SpawnWeapon();
}

void USTUWeaponComponent::SpawnWeapon()
{
    if(!GetWorld()) return;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if(!Character)return;
    
    CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
    if(!CurrentWeapon) return;
    
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
}

