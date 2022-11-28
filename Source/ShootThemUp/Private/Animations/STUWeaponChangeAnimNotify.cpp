// Shoot Them Up Game. All Rights Reserved.


#include "Animations/STUWeaponChangeAnimNotify.h"

void USTUWeaponChangeAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}