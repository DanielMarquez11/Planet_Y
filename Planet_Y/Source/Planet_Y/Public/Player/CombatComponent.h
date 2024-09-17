#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class ABaseWeapon;
class AMainPlayer;
class UCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLANET_Y_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Base Functions
	UCombatComponent();

	void Initialize(AMainPlayer* Player);
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Aim Down Sight
	void AimDownSight();
	void AimDownSightUpdate();
	void StopAiming();
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsAiming = false;
	
	// Equiping And Holstering
	void EquipWeapon() const;
	void HolsterWeapon() const;

	// Shooting
	void Shoot();
	void StopShooting();
	void FireBullet() const;

private:

	// Player References
	UPROPERTY()
	AMainPlayer* MainPlayer;

	UPROPERTY()
	UCharacterMovementComponent* PlayerMovement;

	// Dashing While Aiming
	UPROPERTY(EditAnywhere, Category = "Dash")
	float AimingDashDistance = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float AimingDashTime = 0.20f;

	// Aim Down Sight
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

	UPROPERTY(EditAnywhere, Category = "DefaultMovementValues")
	float AimingPlayerSpeed = 450.0f;
	
	float AimTimeElapsed;

	// Combat
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<ABaseWeapon> StarterWeapon;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	ABaseWeapon* CurrentWeapon;

	FTimerHandle FireBulletTimerHandle;
	float TimeSinceLastFired;

	UPROPERTY(EditDefaultsOnly, Category = "CameraEffects")
	TSubclassOf<UCameraShakeBase> PistolFireEffect;
};

