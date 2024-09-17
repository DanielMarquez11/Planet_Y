#include "Player/PlayerLifeComponent.h"
#include "Levels/Checkpoint.h"
#include "Player/MainPlayer.h"

UPlayerLifeComponent::UPlayerLifeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Health = MaxHealth;
}

void UPlayerLifeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerLifeComponent::Initialize(AMainPlayer* Player)
{
	MainPlayer = Player;
}

void UPlayerLifeComponent::TakeDamage(float Damage)
{
	Health = Health - Damage;

	if (Health <= 0.0f)
	{
		Die();
	}
}

void UPlayerLifeComponent::Die()
{
	if (LastCheckpoint && MainPlayer)
	{
		MainPlayer->SetActorLocation(LastCheckpoint->GetActorLocation());

		Health = MaxHealth;
	}
}

