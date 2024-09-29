// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "GameFramework/PlayerController.h"
#include "STUPlayerController.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class USTURespawnComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
    GENERATED_BODY()
public: ASTUPlayerController();
    
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    USTURespawnComponent* RespawnComponent;

    // Input start
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* MetaMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* PauseGameAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* MuteAction;
    // Input end
    
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    bool bASCInputBound = false;
    
    void OnPauseGame(const FInputActionValue& Value);
    void OnMatchStateChanged(ESTUMatchState State);
    void OnMuteSound(const FInputActionValue& Value);
};