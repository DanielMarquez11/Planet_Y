#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class PLANET_Y_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	
	// Base Functions
	AMainPlayer();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Base Movement
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& Value);
	
	void StartJump();
	void StopJump();

	virtual void Landed(const FHitResult& Hit) override;

	// Dash
	void Dash();
	void StopDash();
	void ResetDashCooldown();
	void CheckDashCollision();

	UPROPERTY(BlueprintReadOnly)
	bool bIsDashing = false;

	// Aim Down Sight
	void AimDownSight();
	void StopAiming();

	UPROPERTY(BlueprintReadOnly)
	bool bIsAiming = false;
	
private:

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UAnimMontage* DoubleJumpMontage;

	// Base Movement
	float DefaultPlayerSpeed = 850.0f;
	float AimingPlayerSpeed = 450.0f;

	// Dash
	float DashDistance;
	float DashTime;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DefaultDashDistance = 900.0f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float AimingDashDistance = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DefaultDashTime = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float AimingDashTime = 0.20f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashCooldown;

	FVector DashEndPoint;
	FVector DashDirection;
	
	float DashTimeElapsed;
	bool bCanDash = true;
	bool bHasAirDashed = false;

	// Aim Down Sight
	UPROPERTY(EditAnywhere, Category = "AimDownSight")
	float DefaultArmLength = 450.0f;

	UPROPERTY(EditAnywhere, Category = "AimDownSight")
	float AimingArmLength = 220.0f;

	UPROPERTY(EditAnywhere, Category = "AimDownSight")
	FVector DefaultCameraOffset = FVector(0.0f, 0.0f, 45.0f);

	UPROPERTY(EditAnywhere, Category = "AimDownSight")
	FVector AimingCameraOffset = FVector(0.0f, 85.0f, 50.0f);

	UPROPERTY(EditAnywhere, Category = "AimDownSight")
	float AimDownSightTime = 0.2f;
	
	float AimTimeElapsed;
};
