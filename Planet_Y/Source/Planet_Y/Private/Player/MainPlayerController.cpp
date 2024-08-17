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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMainPlayerController::HandleStopJump);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleDash);
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

void AMainPlayerController::HandleStopJump()
{
	PlayerRef->StopJump();
}

void AMainPlayerController::HandleDash()
{
	PlayerRef->Dash();
}
