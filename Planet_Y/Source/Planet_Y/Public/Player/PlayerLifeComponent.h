#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerLifeComponent.generated.h"

class AMainPlayer;
class ACheckpoint;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANET_Y_API UPlayerLifeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPlayerLifeComponent();

	virtual void BeginPlay() override;
	void Initialize(AMainPlayer* Player);

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	void Die();

	UPROPERTY(VisibleAnywhere, Category = "PlayerLife")
	ACheckpoint* LastCheckpoint;

private:

	UPROPERTY(EditAnywhere, Category = "PlayerLife")
	float MaxHealth = 100.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "PlayerLife")
	float Health;
	
	UPROPERTY()
	AMainPlayer* MainPlayer;
};

