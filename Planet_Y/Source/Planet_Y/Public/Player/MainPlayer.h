#pragma once

#include "CoreMinimal.h"
#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damageable.h"
#include "MainPlayer.generated.h"

class UPlayerLifeComponent;

UENUM(BlueprintType)
enum class EMovementAbilities : uint8
{
	Moving,
	Dashing,
	WallRunningRight,
	WallRunningLeft
};

class ACheckpoint;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class PLANET_Y_API AMainPlayer : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	
	// Base Functions
	AMainPlayer();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//Player Life
	virtual void TakeDamageToHealth_Implementation(float Damage) override;

	// Base Movement
	UPROPERTY(BlueprintReadOnly)
	EMovementAbilities CurrentMovementAbility;
	
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& Value);
	
	void StartJump();
	void DoubleJump();
	void CoyoteTimeEnded();

	bool bCanJump = true;
	bool bCanDoubleJump = false;
	bool bHasDoubleJumped = false;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	virtual void Landed(const FHitResult& Hit) override;

	// Default Player Values
	UPROPERTY(EditAnywhere, Category = "DefaultMovementValues")
	float DefaultPlayerSpeed = 850.0f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DefaultDashDistance = 900.0f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DefaultDashTime = 0.3f;

	UPROPERTY(EditAnywhere)
	float DefaultArmLength = 450.0f;

	UPROPERTY(EditAnywhere)
	FVector DefaultCameraOffset = FVector(0.0f, 0.0f, 45.0f);
	
	// Dash
	void Dash();	
	void DashUpdate();
	void StopDash();
	void ResetDashCooldown();
	void CheckDashCollision();

	float DashDistance;
	float DashTime;
	
	// Wall Running
	bool WallRunMovement(const FVector& Start, const FVector& End, float Direction);
	void WallRunUpdate();

	void StartWallRun(bool Right, bool Left);
	void EndWallRun(float WallRunCooldown);

	void WallRunJump();
	
	void SupressWallRun(float Cooldown);
	void ResetWallRunSupress();
	bool IsValidWallVector(const FVector& InVector) const;

	bool bIsWallRunning = false;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UPlayerLifeComponent* PlayerLifeComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

private:

	// Base Movement
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UAnimMontage* DoubleJumpMontage;

	UPROPERTY(EditAnywhere, Category = "DefaultMovementValues")
	float BaseGravity = 2.5f;

	UPROPERTY(EditAnywhere, Category = "DefaultMovementValues")
	float PlayerRotationRate = 600.0f;

	UPROPERTY(EditAnywhere, Category = "DefaultMovementValues")
	float JumpHeight = 1100.0f;

	UPROPERTY(EditAnywhere, Category = "DefaultMovementValues")
	float DoubleJumpHeight = 900.0f;

	UPROPERTY(EditAnywhere, Category = "DefaultMovementValues")
	float CoyoteTime = 0.3f;

	// Dash
	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashCooldown;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashEndVelocity = 750.0f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashCollisionCheckDistance = 100.0f;

	FVector DashEndPoint;
	FVector DashDirection;
	
	float DashTimeElapsed;
	bool bCanDash = true;
	bool bHasAirDashed = false;

	// Wall Running
	UPROPERTY(EditAnywhere, Category = "WallRun")
	float WallRunSpeed = 1300.0f;

	UPROPERTY(EditAnywhere, Category = "WallRun")
	float WallRunJumpHeight = 900;

	UPROPERTY(EditAnywhere, Category = "WallRun")
	float WallRunJumpDistance = 500;

	FVector WallRunNormal;
	bool bWallRunSupressed = false;

	// Camera Effects
	UPROPERTY(EditDefaultsOnly, Category = "CameraEffects")
	TSubclassOf<UCameraShakeBase> JumpEffect;

	UPROPERTY(EditDefaultsOnly, Category = "CameraEffects")
	TSubclassOf<UCameraShakeBase> LandEffect;

	UPROPERTY(EditDefaultsOnly, Category = "CameraEffects")
	TSubclassOf<UCameraShakeBase> DoubleJumpEffect;

	UPROPERTY(EditDefaultsOnly, Category = "CameraEffects")
	TSubclassOf<UCameraShakeBase> DashEffect;
};

