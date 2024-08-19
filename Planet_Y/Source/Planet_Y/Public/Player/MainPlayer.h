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

	AMainPlayer();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Base Movement
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& Value);
	void Landed(const FHitResult& Hit) override;

	void StartJump();
	void StopJump();

	// Dash
	void Dash();
	void StopDash();
	void ResetDashCooldown();
	void CheckDashCollision();

	UPROPERTY(BlueprintReadOnly)
	bool bIsDashing = false;
	
private:

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UAnimMontage* DoubleJumpMontage;

	// Dash
	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashDistance;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashTime;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashCooldown;

	FVector DashEndPoint;
	FVector DashDirection;
	float DashTimeElapsed;
	
	bool bCanDash = true;
	bool bHasAirDashed = false;
};
