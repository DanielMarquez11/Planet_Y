#include "Player/MainPlayerController.h"
#include "Player/MainPlayer.h"

AMainPlayerController::AMainPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AMainPlayerController::HandleMove(const FInputActionValue& InputActionValue)
{
}

void AMainPlayerController::HandleMove()
{
}

void AMainPlayerController::HandleDash()
{
}
