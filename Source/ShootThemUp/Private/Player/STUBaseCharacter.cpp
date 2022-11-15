// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/STUBaseCharacter.h"
#include "AnimGraphRuntime/Public/KismetAnimationLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All)

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
: Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const auto Health = HealthComponent->GetHealth();
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));

    TakeDamage(0.1f, FDamageEvent(), Controller, this);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning);
}

bool ASTUBaseCharacter::IsRunning() const
{
    return bWantsToRun && bIsMovingForward && !GetVelocity().IsZero();
}

float ASTUBaseCharacter::GetMoveDirection() const
{
    float Direction = UKismetAnimationLibrary::CalculateDirection(GetVelocity(),GetActorRotation());
    // if(GEngine)  GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, FString::Printf(TEXT("Direction: %f"), Direction));
    return Direction;
}

void ASTUBaseCharacter::MoveForward(float Amount)
{
    bIsMovingForward = Amount > 0.0f;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::OnStartRunning()
{
    bWantsToRun = true;
}

void ASTUBaseCharacter::OnStopRunning()
{
    bWantsToRun = false;
}