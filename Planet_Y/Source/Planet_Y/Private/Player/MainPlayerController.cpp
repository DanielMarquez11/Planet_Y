#include "Player/MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Player/MainPlayer.h"

AMainPlayerController::AMainPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerContext, 0);
	}

	if (AMainPlayer* MyCharacter = Cast<AMainPlayer>(GetPawn()))
	{
		PlayerRef = MyCharacter;
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleLook);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMainPlayerController::HandleStartJump);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleDash);
		EnhancedInputComponent->BindAction(AimDownSightAction, ETriggerEvent::Started, this, &AMainPlayerController::HandleAimDownSight);
		EnhancedInputComponent->BindAction(AimDownSightAction, ETriggerEvent::Completed, this, &AMainPlayerController::HandleStopAiming);
		
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleStartShooting);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &AMainPlayerController::HandleStopShooting);
	}
}

void AMainPlayerController::HandleMove(const FInputActionValue& InputActionValue)
{
	PlayerRef->Move(InputActionValue);
}

void AMainPlayerController::HandleLook(const FInputActionValue& InputActionValue)
{
	PlayerRef->Look(InputActionValue);
}

void AMainPlayerController::HandleStartJump()
{
	PlayerRef->StartJump();
}

void AMainPlayerController::HandleDash()
{
	PlayerRef->Dash();
}

void AMainPlayerController::HandleAimDownSight()
{
	PlayerRef->AimDownSight();
}

void AMainPlayerController::HandleStopAiming()
{
	PlayerRef->StopAiming();
}

void AMainPlayerController::HandleStartShooting()
{
	PlayerRef->Shoot();
}

void AMainPlayerController::HandleStopShooting()
{
	PlayerRef->StopShooting();
}
