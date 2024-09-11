#include "Enemies/BaseEnemy.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}

void ABaseEnemy::TakeDamageToHealth_Implementation(float Damage)
{
	Health = Health - Damage;
	
	if (Health <= 0.0f)
	{
		Execute_Die(this);
	}
}

void ABaseEnemy::Die_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, "Enemy Got Killed");
}

