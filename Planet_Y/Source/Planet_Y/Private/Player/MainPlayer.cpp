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

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

