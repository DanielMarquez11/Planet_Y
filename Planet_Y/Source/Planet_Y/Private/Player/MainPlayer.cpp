#include "Player/MainPlayer.h"

AMainPlayer::AMainPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainPlayer::Move(const FInputActionValue& InputActionValue) const
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, "Move");
}

void AMainPlayer::StartJump()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, "Start Jump");
}

void AMainPlayer::StopJump()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, "Stop Jump");
}

void AMainPlayer::Dash()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, "Dash");
}


