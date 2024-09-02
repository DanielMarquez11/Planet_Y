#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class PLANET_Y_API IDamageable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TakeDamageToHealth(float Damage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Die();
};
