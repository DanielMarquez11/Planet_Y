#pragma once

#include "CoreMinimal.h"
#include "Enemies/BaseEnemy.h"
#include "BaseBoss.generated.h"

UCLASS()
class PLANET_Y_API ABaseBoss : public ABaseEnemy
{
	GENERATED_BODY()

	// Here will be all the logic that is shared across all bosses. For example enabling and updating its boss healthbar
};
