#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

struct FInputActionValue;

UCLASS()
class PLANET_Y_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:

	AMainPlayer();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Base Movement
	void Move(const FInputActionValue& InputActionValue) const;

	void StartJump();
	void StopJump();

	void Dash();
	
private:

	
};
