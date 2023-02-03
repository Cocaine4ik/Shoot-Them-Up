// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUAmbientSound.generated.h"

class UAudioComponent;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTUAmbientSound : public AActor
{
    GENERATED_BODY()

public:
    ASTUAmbientSound();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UAudioComponent* AudioComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (ClampMin = 0.0f, ClampMax = 20.0f))
    float FadeDuration = 2.0f;
    
    virtual void BeginPlay() override;

};