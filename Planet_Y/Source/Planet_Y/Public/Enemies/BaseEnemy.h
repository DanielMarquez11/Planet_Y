#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damageable.h"
#include "BaseEnemy.generated.h"

UCLASS()
class PLANET_Y_API ABaseEnemy : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:

	ABaseEnemy();

	virtual void BeginPlay() override;

protected:

	// Enemy Life
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerLife")
	float MaxHealth;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerLife")
	float Health;

	virtual void TakeDamageToHealth_Implementation(float Damage) override;
	void Die();
};

