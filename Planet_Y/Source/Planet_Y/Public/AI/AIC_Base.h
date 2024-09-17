// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_Base.generated.h"

UENUM(BlueprintType)
enum EAbilities
{
	None,
	DefaultAttack,
	Leapen,
	CrystalLine,
	CrystalLineFlurry,
	Roar,
	Dead
};

UCLASS()
class PLANET_Y_API AAIC_Base : public AAIController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetState(EAbilities State);

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	 TEnumAsByte<EAbilities> CurrentState;
};
