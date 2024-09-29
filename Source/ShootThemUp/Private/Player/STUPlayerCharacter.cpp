// Shoot Them Up Game. All Rights Reserved.

#include "Player/STUPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "STUPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void ASTUPlayerCharacter::OnDeath()
{
    Super::OnDeath();
    
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void ASTUPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(CameraCollisionComponent)

    //Add Input Mapping Context
    if (const ASTUPlayerController* PlayerController = Cast<ASTUPlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
            PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(PlayerMappingContext, 0);
        }
    }
    
    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionEndOverlap);
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent)
    check(WeaponComponent)

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::MoveForward);
        EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::MoveRight);
        EnhancedInputComponent->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::LookUp);
        EnhancedInputComponent->BindAction(TurnAroundAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::TurnAround);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASTUPlayerCharacter::Jump);
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ASTUPlayerCharacter::OnStartRunning);
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ASTUPlayerCharacter::OnStopRunning);
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, WeaponComponent, &USTUWeaponComponent::StartFire);
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, WeaponComponent, &USTUWeaponComponent::StopFire);
        EnhancedInputComponent->BindAction(NextWeaponAction, ETriggerEvent::Started, WeaponComponent, &USTUWeaponComponent::NextWeapon);
        EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, WeaponComponent, &USTUWeaponComponent::Reload);
        EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, WeaponComponent, &USTUWeaponComponent::Zoom, true);
        EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, WeaponComponent, &USTUWeaponComponent::Zoom, false);
    }
}

bool ASTUPlayerCharacter::IsRunning() const
{
    return bWantsToRun && bIsMovingForward && !GetVelocity().IsZero();
}

void ASTUPlayerCharacter::MoveForward(const FInputActionValue& Value)
{
    const float Amount = Value.Get<float>();
    
    bIsMovingForward = Amount > 0.0f;
    if(Amount == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), Amount);

    if(IsRunning() && WeaponComponent->IsFiring())
    {
        WeaponComponent->StopFire();
    }
}

void ASTUPlayerCharacter::MoveRight(const FInputActionValue& Value)
{
    const float Amount = Value.Get<float>();
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUPlayerCharacter::LookUp(const FInputActionValue& Value)
{
    const float Amount = Value.Get<float>();
    AddControllerPitchInput(Amount);
}

void ASTUPlayerCharacter::TurnAround(const FInputActionValue& Value)
{
    const float Amount = Value.Get<float>();
    AddControllerYawInput(Amount);
}

void ASTUPlayerCharacter::OnStartRunning()
{
    bWantsToRun = true;
    if(IsRunning())
    {
        WeaponComponent->StopFire();
    }
}

void ASTUPlayerCharacter::OnStopRunning()
{
    bWantsToRun = false;
}

void ASTUPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::CheckCameraOverlap()
{
    const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(HideMesh);

    TArray<USceneComponent*> MeshChildren;
    GetMesh()->GetChildrenComponents(true, MeshChildren);

    for (auto MeshChild : MeshChildren)
    {
        const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
        if(MeshChildGeometry)
        {
            MeshChildGeometry->SetOwnerNoSee(HideMesh);
        }
    }
}