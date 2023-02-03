// Shoot Them Up Game. All Rights Reserved.

#include "Sound/STUAmbientSound.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUAmbientSound, All, All)
ASTUAmbientSound::ASTUAmbientSound()
{
    PrimaryActorTick.bCanEverTick = false;

    AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
    AudioComponent->SetupAttachment(GetRootComponent());
}

void ASTUAmbientSound::BeginPlay()
{
    Super::BeginPlay();
    
        if(!AudioComponent) return;
    
        AudioComponent->FadeIn(FadeDuration);
        
        UE_LOG(LogSTUAmbientSound, Display, TEXT("Start ambient sound: %s"), *AudioComponent->Sound->GetName())
}