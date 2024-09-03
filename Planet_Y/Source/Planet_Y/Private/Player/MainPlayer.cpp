#include "Player/MainPlayer.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Combat/Bullets/BaseBullet.h"
#include "Combat/Weapons/Pistol.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#pragma region Base Functions
AMainPlayer::AMainPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Disable character rotation based on controller input
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false; 
	bUseControllerRotationRoll = false;

	// Initialize Capsule Component
	GetCapsuleComponent()->InitCapsuleSize(35.f, 60.0f);

	// Configure Jumping
	JumpMaxHoldTime = 0.1f;
	JumpMaxCount = 2;

	// Initialize Dashing
	DashDistance = DefaultDashDistance;
	DashTime = DefaultDashTime;
	
	// Configure Character Movement
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, PlayerRotationRate, 0.0f); 
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	GetCharacterMovement()->GravityScale = BaseGravity;
	GetCharacterMovement()->MaxAcceleration = 2300.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;

	GetCharacterMovement()->MaxWalkSpeed = DefaultPlayerSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = 4000.0f;

	GetCharacterMovement()->JumpZVelocity = JumpHeight;
	GetCharacterMovement()->BrakingDecelerationFalling = 100.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	// Initialize Camera Boom (Spring Arm)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = DefaultArmLength; 
	CameraBoom->SocketOffset = DefaultCameraOffset; 
	CameraBoom->bUsePawnControlRotation = true; 

	// Initialize Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	
	FollowCamera->bUsePawnControlRotation = false;

	// Player Life
	Health = MaxHealth;
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (StarterWeapon)
	{
		 CurrentWeapon = GetWorld()->SpawnActor<ABaseWeapon>(StarterWeapon);

		 const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		 CurrentWeapon->AttachToComponent(GetMesh(), AttachmentRules, FName("PistolHolster"));
	}
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WallRunUpdate();
	DashUpdate();
	AimDownSightUpdate();
}
#pragma endregion Base Functions

#pragma region Player Life
void AMainPlayer::TakeDamageToHealth_Implementation(float Damage)
{
	Health = Health - Damage;

	if (Health <= 0.0f)
	{
		Execute_Die(this);
	}
}

void AMainPlayer::Die_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, "Player Dead");
}
#pragma endregion Player Life

#pragma region Base Movement
void AMainPlayer::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMainPlayer::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMainPlayer::StartJump()
{
	if (bIsAiming) {return;}

	if (bIsWallRunning)
	{
		WallRunJump();
		return;
	}

	if (JumpCurrentCount == 1)
	{
		const FVector BoostedVelocity = GetCharacterMovement()->Velocity * 0.4f + GetLastMovementInputVector().GetSafeNormal() * DoubleJumpHeight;
		GetCharacterMovement()->Velocity = BoostedVelocity;

		PlayAnimMontage(DoubleJumpMontage);
	}

	Jump();
}

void AMainPlayer::StopJump()
{
	StopJumping();
}

void AMainPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	bHasAirDashed = false;
	EndWallRun(0.0f);
}
#pragma endregion Base Movement

#pragma region Dash
void AMainPlayer::Dash()
{
	if (bIsDashing || !bCanDash)
	{
		return;
	}
	
	const bool bIsInAir = !GetCharacterMovement()->IsMovingOnGround();

	if (bIsInAir && bHasAirDashed)
	{
		return;
	}

	bIsDashing = true;
	bCanDash = false;
	bHasAirDashed = bIsInAir;

	DashTimeElapsed = 0.0f;

	// Stop current movement and disable gravity during dash
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->GravityScale = 0;

	const FVector MovementInput = GetLastMovementInputVector();
	const FVector DashVector = MovementInput.IsNearlyZero() ? GetActorForwardVector() : MovementInput.GetSafeNormal();

	DashEndPoint = GetActorLocation() + DashVector * DashDistance;
	DashDirection = DashVector;

	FTimerHandle DashTimer;
	FTimerHandle DashCooldownTimer;
		
	GetWorld()->GetTimerManager().SetTimer(DashTimer, this, &AMainPlayer::StopDash, DashTime, false);
	GetWorld()->GetTimerManager().SetTimer(DashCooldownTimer, this, &AMainPlayer::ResetDashCooldown, DashCooldown, false);
}

void AMainPlayer::DashUpdate()
{
	if (!bIsDashing) return;
	
	const float LerpAlpha = (DashTimeElapsed + GetWorld()->GetDeltaSeconds()) / DashTime;
	const FVector DashUpdateLocation = FMath::Lerp(GetActorLocation(), DashEndPoint, LerpAlpha);

	CheckDashCollision();
	SetActorLocation(DashUpdateLocation);
}

void AMainPlayer::StopDash()
{
	GetCharacterMovement()->GravityScale = BaseGravity;
	GetCharacterMovement()->Velocity = DashDirection * DashEndVelocity;
	
	bIsDashing = false;
}

void AMainPlayer::ResetDashCooldown()
{
	bCanDash = true;
}

void AMainPlayer::CheckDashCollision()
{
	const FVector TraceStartLocation = GetActorLocation();
	const FVector ForwardVector = GetLastMovementInputVector().GetSafeNormal();
	const FVector TraceEndLocation = TraceStartLocation + ForwardVector * DashCollisionCheckDistance;

	FHitResult HitResult;
	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStartLocation, TraceEndLocation, ECC_Visibility))
	{
		StopDash();
	}
}
#pragma endregion Dash

#pragma region Wallrun
void AMainPlayer::WallRunUpdate()
{
	if (bWallRunSupressed || bIsAiming) return;

	const FVector ActorLocation = GetActorLocation();
	const FVector RightVector = GetActorRightVector();
	const FVector ForwardVector = GetActorForwardVector();
	
	const FVector WallRunRightVector = ActorLocation + (RightVector * 50.0f) + (ForwardVector);
	const FVector WallRunLeftVector = ActorLocation + (RightVector * -50.0f) + (ForwardVector);

	const bool bRightWallRun = WallRunMovement(ActorLocation, WallRunRightVector, -1.0f);
	const bool bLeftWallRun = !bIsWallRunningRight && WallRunMovement(ActorLocation, WallRunLeftVector, 1.0f);
	
	if (bRightWallRun)
	{
		StartWallRun(true, false);
	}
	else if (bLeftWallRun)
	{
		StartWallRun(false, true); 
	}
	else
	{
		EndWallRun(1.0f); 
	}
}

void AMainPlayer::StartWallRun(const bool Right, const bool Left)
{
	bIsWallRunning = true;
	bIsWallRunningRight = Right;
	bIsWallRunningLeft = Left;
	bCanDash = false;

	StopDash();

	// Set movement values when start wall run
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	
	Movement->GravityScale = 0.0f;
	Movement->bOrientRotationToMovement = false;
	Movement->bUseControllerDesiredRotation = false;
}

bool AMainPlayer::WallRunMovement(const FVector& Start, const FVector& End, float Direction)
{
	FHitResult Hit;
	
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);

	if (Hit.bBlockingHit && Hit.GetActor()->ActorHasTag("RunnableWall"))
	{
		if (IsValidWallVector(Hit.Normal) && GetCharacterMovement()->IsFalling())
		{
			WallRunNormal = Hit.Normal;
			
			const FVector LaunchVector = WallRunNormal - GetActorLocation();
			const FVector PlayerToWallVector = WallRunNormal * LaunchVector.Length();
			const FVector ForwardVector = WallRunNormal.Cross(FVector(0, 0, 1)) * (WallRunSpeed * Direction);

			LaunchCharacter(PlayerToWallVector, false, false);
			LaunchCharacter(ForwardVector, true, true);
			
			if (!ForwardVector.IsZero())
			{
				const FRotator TargetRotation = FRotationMatrix::MakeFromX(ForwardVector.GetSafeNormal()).Rotator();
				const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), AimDownSightSpeed);

				SetActorRotation(NewRotation);

				return true;
			}
		}
	}
	return false;
}

void AMainPlayer::EndWallRun(float WallRunCooldown)
{
	if (!bIsWallRunning) return;

	// End wall running state
	bIsWallRunning = false;
	bIsWallRunningRight = false;
	bIsWallRunningLeft = false;

	// Reset double jump and dash state
	bCanDash = true;
	bHasAirDashed = false;
	JumpCurrentCount = 1;

	// Restore movement settings
	UCharacterMovementComponent* PlayerMovement = GetCharacterMovement();
	
	PlayerMovement->bOrientRotationToMovement = true;
	PlayerMovement->bUseControllerDesiredRotation = true;
		
	if (!bIsDashing)
	{
		PlayerMovement->GravityScale = BaseGravity;
	}
	
	SupressWallRun(WallRunCooldown);
}

void AMainPlayer::WallRunJump()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, "Hit");
	EndWallRun(0.35f);

	const FVector JumpVector = WallRunJumpDistance * FVector(WallRunNormal.X, WallRunNormal.Y, 0) + FVector(0, 0, WallRunJumpHeight);
	LaunchCharacter(JumpVector, false, true);
}

bool AMainPlayer::IsValidWallVector(const FVector& InVector) const
{
	return FMath::Abs(InVector.Z) <= 0.52f && GetCharacterMovement()->IsFalling();
}

void AMainPlayer::SupressWallRun(float WallRunCooldown)
{
	bWallRunSupressed = true;

	FTimerHandle WallRunSupressTimer;
	GetWorld()->GetTimerManager().SetTimer(WallRunSupressTimer, this, &AMainPlayer::ResetWallRunSupress, WallRunCooldown);
}

void AMainPlayer::ResetWallRunSupress()
{
	bWallRunSupressed = false;
}
#pragma endregion Wallrun

#pragma region Aim Down Sight
void AMainPlayer::AimDownSight()
{
	if (CurrentWeapon == nullptr) {return;}

	if (bIsWallRunning)
	{
		EndWallRun(0.2f);
	}
	
	bIsAiming = true;

	EquipWeapon();

	// Update dash parameters for aiming
	DashDistance = AimingDashDistance;
	DashTime = AimingDashTime;

	// Adjust movement settings for aiming
	UCharacterMovementComponent* PlayerMovement = GetCharacterMovement();
	
	PlayerMovement->MaxWalkSpeed = AimingPlayerSpeed;
	PlayerMovement->bOrientRotationToMovement = false;
}

void AMainPlayer::AimDownSightUpdate()
{
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	const float LerpAlpha = FMath::Clamp((AimTimeElapsed + DeltaTime) / AimDownSightTime, 0.0f, 1.0f);

	// Determine the target values based on aiming state
	const float TargetArmLength = bIsAiming ? AimingArmLength : DefaultArmLength;
	const FVector TargetSocketOffset = bIsAiming ? AimingCameraOffset : DefaultCameraOffset;

	// Update AimTimeElapsed
	if (bIsAiming || LerpAlpha > 0.0f)
	{
		CameraBoom->TargetArmLength = FMath::Lerp(CameraBoom->TargetArmLength, TargetArmLength, LerpAlpha);
		CameraBoom->SocketOffset = FMath::Lerp(CameraBoom->SocketOffset, TargetSocketOffset, LerpAlpha);
		
		// Reset AimTimeElapsed when the lerp is complete and not aiming
		if (!bIsAiming && LerpAlpha >= 1.0f)
		{
			AimTimeElapsed = 0.0f;
		}
	}
}

void AMainPlayer::StopAiming()
{
	bIsAiming = false;

	HolsterWeapon();

	// Reset dash parameters
	DashDistance = DefaultDashDistance;
	DashTime = DefaultDashTime;

	// Adjust movement settings to default
	UCharacterMovementComponent* PlayerMovement = GetCharacterMovement();

	PlayerMovement->MaxWalkSpeed = DefaultPlayerSpeed;
	PlayerMovement->bOrientRotationToMovement = true;
}

void AMainPlayer::EquipWeapon() const
{
	if (CurrentWeapon)
	{
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		CurrentWeapon->AttachToComponent(GetMesh(), AttachmentRules, FName("WeaponSocket"));
	}
}

void AMainPlayer::HolsterWeapon() const
{
	if (CurrentWeapon)
	{
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		CurrentWeapon->AttachToComponent(GetMesh(), AttachmentRules, FName("PistolHolster"));
	}
}

#pragma endregion Aim Down Sight

#pragma region Shooting
void AMainPlayer::Shoot()
{
	if (CurrentWeapon && CurrentWeapon->WeaponBullet && bIsAiming)
	{
		const float CurrentTime = GetWorld()->GetTimeSeconds();

		if (CurrentTime - TimeSinceLastFired >= CurrentWeapon->FireRate)
		{
			FireBullet();
			TimeSinceLastFired = CurrentTime;

			GetWorld()->GetTimerManager().SetTimer(FireBulletTimerHandle, this, &AMainPlayer::FireBullet, CurrentWeapon->FireRate, true);
		}
	}
}

void AMainPlayer::StopShooting()
{
	GetWorld()->GetTimerManager().ClearTimer(FireBulletTimerHandle);
}

void AMainPlayer::FireBullet() const
{
	const APlayerCameraManager* PlayerCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	const FVector Start = PlayerCamera->GetCameraLocation() + (PlayerCamera->GetCameraRotation().Vector() * 300.0f);
	const FVector End = (PlayerCamera->GetCameraRotation().Vector() * 10000.0f) + Start;

	FHitResult Hit;

	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);

	const FVector TargetPoint = bHit ? Hit.Location : End;
	const FVector Direction = (TargetPoint - CurrentWeapon->BulletSpawnPoint->GetComponentLocation()).GetSafeNormal();

	GetWorld()->SpawnActor<ABaseBullet>(CurrentWeapon->WeaponBullet, CurrentWeapon->BulletSpawnPoint->GetComponentLocation(), Direction.Rotation());
}
#pragma endregion Shooting

