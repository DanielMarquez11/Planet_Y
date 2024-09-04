#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class AMainPlayer;

UCLASS()
class PLANET_Y_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMainPlayerController();

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> PlayerContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> DashAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AimDownSightAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShootAction;

private:

	// Input Handlers
	void HandleMove(const FInputActionValue& InputActionValue);
	void HandleLook(const FInputActionValue& InputActionValue);
	
	void HandleStartJump();
	void HandleDash();

	void HandleAimDownSight();
	void HandleStopAiming();

	void HandleStartShooting();
	void HandleStopShooting();

	UPROPERTY()
	AMainPlayer* PlayerRef;
};

