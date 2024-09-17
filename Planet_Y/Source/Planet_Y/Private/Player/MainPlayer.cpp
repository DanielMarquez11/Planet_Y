#include "Player/MainPlayer.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CombatComponent.h"
#include "Player/PlayerLifeComponent.h"

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

	// Player Components
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	PlayerLifeComponent = CreateDefaultSubobject<UPlayerLifeComponent>(TEXT("PlayerLifeComponent"));
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	CombatComponent->Initialize(this);
	PlayerLifeComponent->Initialize(this);
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WallRunUpdate();
	DashUpdate();
}

void AMainPlayer::TakeDamageToHealth_Implementation(float Damage) {PlayerLifeComponent->TakeDamage(Damage);}
#pragma endregion Base Functions

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
	if (CombatComponent->bIsAiming) {return;}

	if (bIsWallRunning)
	{
		bCanJump = false;
		bCanDoubleJump = true;
		bHasDoubleJumped = false;
		
		WallRunJump();

		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(JumpEffect, 1.0f);
		return;
	}

	if (bCanJump)
	{
		LaunchCharacter(FVector(0.0f, 0.0f, JumpHeight), false, true);

		bCanJump = false;
		bCanDoubleJump = true;

		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(JumpEffect, 1.0f);
	}
	else if (bCanDoubleJump && CurrentMovementAbility != EMovementAbilities::Dashing)
	{
		DoubleJump();
	}
}

void AMainPlayer::DoubleJump()
{
	bCanDoubleJump = false;
	bHasDoubleJumped = true;
	
	const FVector BoostedVelocity = GetCharacterMovement()->Velocity * 0.4f + GetLastMovementInputVector().GetSafeNormal() * 800.0f;
	GetCharacterMovement()->Velocity = BoostedVelocity;

	PlayAnimMontage(DoubleJumpMontage);
	
	LaunchCharacter(FVector(0.0f, 0.0f, DoubleJumpHeight), false, true);

	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(DoubleJumpEffect, 1.0f);
}

void AMainPlayer::CoyoteTimeEnded()
{
	bCanJump = false;

	if (!bHasDoubleJumped) { bCanDoubleJump = true; }
}

void AMainPlayer::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		FTimerHandle CoyoteTimer;
		GetWorld()->GetTimerManager().SetTimer(CoyoteTimer, this, &AMainPlayer::CoyoteTimeEnded, CoyoteTime, false);

		GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f); 
	}
}

void AMainPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	bCanJump = true;
	bHasDoubleJumped = false;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, PlayerRotationRate, 0.0f); 
	
	bHasAirDashed = false;
	EndWallRun(0.0f);
	
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(LandEffect, 1.0f);
}
#pragma endregion Base Movement

#pragma region Dash
void AMainPlayer::Dash()
{
	const bool bIsInAir = !GetCharacterMovement()->IsMovingOnGround();
	
	if (CurrentMovementAbility == EMovementAbilities::Dashing || !bCanDash) { return; }
	if (bIsInAir && bHasAirDashed) { return; }

	CurrentMovementAbility = EMovementAbilities::Dashing;
	
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

	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(DashEffect, 1.0f);
}

void AMainPlayer::DashUpdate()
{
	if (CurrentMovementAbility != EMovementAbilities::Dashing) return;
	
	const float LerpAlpha = (DashTimeElapsed + GetWorld()->GetDeltaSeconds()) / DashTime;
	const FVector DashUpdateLocation = FMath::Lerp(GetActorLocation(), DashEndPoint, LerpAlpha);

	CheckDashCollision();
	SetActorLocation(DashUpdateLocation);
}

void AMainPlayer::StopDash()
{
	GetCharacterMovement()->GravityScale = BaseGravity;
	GetCharacterMovement()->Velocity = DashDirection * DashEndVelocity;
	
	CurrentMovementAbility = EMovementAbilities::Moving;
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
	if (bWallRunSupressed || CombatComponent->bIsAiming) return;

	const FVector ActorLocation = GetActorLocation();
	const FVector RightVector = GetActorRightVector();
	const FVector ForwardVector = GetActorForwardVector();
	
	const FVector WallRunRightVector = ActorLocation + (RightVector * 50.0f) + (ForwardVector);
	const FVector WallRunLeftVector = ActorLocation + (RightVector * -50.0f) + (ForwardVector);

	const bool bRightWallRun = WallRunMovement(ActorLocation, WallRunRightVector, -1.0f);
	const bool bLeftWallRun = CurrentMovementAbility != EMovementAbilities::WallRunningRight && WallRunMovement(ActorLocation, WallRunLeftVector, 1.0f);
	
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
	bCanDash = false;
	StopDash();
	
	bIsWallRunning = true;

	if (Right) {CurrentMovementAbility = EMovementAbilities::WallRunningRight; }
	if (Left) {CurrentMovementAbility = EMovementAbilities::WallRunningLeft; }

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
				const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f);

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
	CurrentMovementAbility = EMovementAbilities::Moving;

	// Reset double jump and dash state
	bCanDash = true;
	bHasAirDashed = false;
	JumpCurrentCount = 1;

	// Restore movement settings
	UCharacterMovementComponent* PlayerMovement = GetCharacterMovement();
	
	PlayerMovement->bOrientRotationToMovement = true;
	PlayerMovement->bUseControllerDesiredRotation = true;
		
	if (CurrentMovementAbility != EMovementAbilities::Dashing)
	{
		PlayerMovement->GravityScale = BaseGravity;
	}
	
	SupressWallRun(WallRunCooldown);
}

void AMainPlayer::WallRunJump()
{
	EndWallRun(0.35f);

	const FVector LaunchVector = FVector(WallRunNormal.X * WallRunJumpDistance, WallRunNormal.Y * WallRunJumpDistance, WallRunJumpHeight) + GetActorForwardVector() * 1000.0f;

	LaunchCharacter(LaunchVector, true, true);
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

