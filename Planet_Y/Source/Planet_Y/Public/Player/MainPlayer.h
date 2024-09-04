#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damageable.h"
#include "MainPlayer.generated.h"

class ABaseWeapon;
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
	virtual void Die_Implementation() override;

	// Base Movement
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

	// Dash
	void Dash();	
	void DashUpdate();
	void StopDash();
	void ResetDashCooldown();
	void CheckDashCollision();

	UPROPERTY(BlueprintReadOnly)
	bool bIsDashing = false;

	// Wall Running
	bool WallRunMovement(const FVector& Start, const FVector& End, float Direction);
	void WallRunUpdate();

	void StartWallRun(bool Right, bool Left);
	void EndWallRun(float WallRunCooldown);

	void WallRunJump();
	
	void SupressWallRun(float Cooldown);
	void ResetWallRunSupress();
	
	bool IsValidWallVector(const FVector& InVector) const;

	UPROPERTY(BlueprintReadOnly)
	bool bIsWallRunningLeft = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsWallRunningRight = false;

	// Aim Down Sight
	void AimDownSight();
	void AimDownSightUpdate();
	void StopAiming();

	UPROPERTY(BlueprintReadOnly)
	bool bIsAiming = false;

	// Combat
	void EquipWeapon() const;
	void HolsterWeapon() const;
	
	void Shoot();
	void StopShooting();
	
	void FireBullet() const;
	
private:

	// Player Life
	UPROPERTY(EditAnywhere, Category = "PlayerLife")
	float MaxHealth = 100.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "PlayerLife")
	float Health;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UAnimMontage* DoubleJumpMontage;

	// Base Movement
	UPROPERTY(EditAnywhere, Category = "DefaultMovementValues")
	float BaseGravity = 2.5f;

	UPROPERTY(EditAnywhere, Category = "DefaultMovementValues")
	float PlayerRotationRate = 600.0f;

	UPROPERTY(EditAnywhere, Category = "DefaultMovementValues")
	float DefaultPlayerSpeed = 850.0f;

	UPROPERTY(EditAnywhere, Category = "DefaultMovementValues")
	float AimingPlayerSpeed = 450.0f;

	// Jumps
	UPROPERTY(EditAnywhere, Category = "DefaultMovementValues")
	float JumpHeight = 1100.0f;

	UPROPERTY(EditAnywhere, Category = "DefaultMovementValues")
	float DoubleJumpHeight = 900.0f;

	UPROPERTY(EditAnywhere, Category = "DefaultMovementValues")
	float CoyoteTime = 0.3f;

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
	bool bTurnPlayerForward = false;
	bool bIsWallRunning = false;

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

	UPROPERTY(EditAnywhere, Category = "AimDownSight")
	float AimDownSightSpeed = 5.0f;
	
	float AimTimeElapsed;

	// Combat
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<ABaseWeapon> StarterWeapon;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	ABaseWeapon* CurrentWeapon;

	FTimerHandle FireBulletTimerHandle;
	float TimeSinceLastFired;

	// Camera Effects
	UPROPERTY(EditDefaultsOnly, Category = "CameraEffects")
	TSubclassOf<UCameraShakeBase> PistolFireEffect;
};
