// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "STUPlayerStart.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerStart : public APlayerStart
{
    GENERATED_BODY()
    
public:
    
    void SetTeamID(const int32 ID) { TeamID =  ID; }
    int32 GetTeamID() const { return TeamID; }
    
protected:
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Game Data")
    int32 TeamID = 1;
};