#include "Player/CombatComponent.h"
#include "Combat/Bullets/BaseBullet.h"
#include "Combat/Weapons/BaseWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/MainPlayer.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::Initialize(AMainPlayer* Player)
{
	MainPlayer = Player;
	PlayerMovement = MainPlayer->GetCharacterMovement();
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (StarterWeapon)
	{
		CurrentWeapon = GetWorld()->SpawnActor<ABaseWeapon>(StarterWeapon);

		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		CurrentWeapon->AttachToComponent(MainPlayer->GetMesh(), AttachmentRules, FName("PistolHolster"));
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AimDownSightUpdate();
}

#pragma region Aiming
void UCombatComponent::AimDownSight()
{
	if (CurrentWeapon == nullptr) {return;}

	if (MainPlayer->bIsWallRunning)
	{
		MainPlayer->EndWallRun(0.2f);
	}
	
	bIsAiming = true;

	EquipWeapon();

	// Update dash parameters for aiming
	MainPlayer->DashDistance = AimingDashDistance;
	MainPlayer->DashTime = AimingDashTime;

	// Adjust movement settings for aiming
	PlayerMovement->MaxWalkSpeed = AimingPlayerSpeed;
	PlayerMovement->bOrientRotationToMovement = false;
}

void UCombatComponent::AimDownSightUpdate()
{
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	const float LerpAlpha = FMath::Clamp((AimTimeElapsed + DeltaTime) / AimDownSightTime, 0.0f, 1.0f);

	// Determine the target values based on aiming state
	const float TargetArmLength = bIsAiming ? AimingArmLength : MainPlayer->DefaultArmLength;
	const FVector TargetSocketOffset = bIsAiming ? AimingCameraOffset : DefaultCameraOffset;

	// Update AimTimeElapsed
	if (bIsAiming || LerpAlpha > 0.0f)
	{
		MainPlayer->CameraBoom->TargetArmLength = FMath::Lerp(MainPlayer->CameraBoom->TargetArmLength, TargetArmLength, LerpAlpha);
		MainPlayer->CameraBoom->SocketOffset = FMath::Lerp(MainPlayer->CameraBoom->SocketOffset, TargetSocketOffset, LerpAlpha);
		
		// Reset AimTimeElapsed when the lerp is complete and not aiming
		if (!bIsAiming && LerpAlpha >= 1.0f)
		{
			AimTimeElapsed = 0.0f;
		}
	}
}

void UCombatComponent::StopAiming()
{
	bIsAiming = false;

	HolsterWeapon();

	// Reset dash parameters
	MainPlayer->DashDistance = MainPlayer->DefaultDashDistance;
	MainPlayer->DashTime = MainPlayer->DefaultDashTime;

	PlayerMovement->MaxWalkSpeed = MainPlayer->DefaultPlayerSpeed;
	PlayerMovement->bOrientRotationToMovement = true;
}
#pragma endregion Aiming

#pragma region Shooting
void UCombatComponent::Shoot()
{
	if (CurrentWeapon && CurrentWeapon->WeaponBullet && bIsAiming)
	{
		const float CurrentTime = GetWorld()->GetTimeSeconds();

		if (CurrentTime - TimeSinceLastFired >= CurrentWeapon->FireRate)
		{
			FireBullet();
			TimeSinceLastFired = CurrentTime;

			GetWorld()->GetTimerManager().SetTimer(FireBulletTimerHandle, this, &UCombatComponent::FireBullet, CurrentWeapon->FireRate, true);
		}
	}
}

void UCombatComponent::StopShooting()
{
	GetWorld()->GetTimerManager().ClearTimer(FireBulletTimerHandle);
}

void UCombatComponent::FireBullet() const
{
	const APlayerCameraManager* PlayerCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	
	const FVector Start = PlayerCamera->GetCameraLocation() + (PlayerCamera->GetCameraRotation().Vector() * 300.0f);
	const FVector End = (PlayerCamera->GetCameraRotation().Vector() * 10000.0f) + Start;
	
	FHitResult Hit;
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);
	
	const FVector TargetPoint = bHit ? Hit.Location : End;
	const FVector Direction = (TargetPoint - CurrentWeapon->BulletSpawnPoint->GetComponentLocation()).GetSafeNormal();

	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Instigator = MainPlayer;   // Setting the player as the instigator
	ActorSpawnParameters.Owner = MainPlayer;

	GetWorld()->SpawnActor<ABaseBullet>(CurrentWeapon->WeaponBullet, CurrentWeapon->BulletSpawnPoint->GetComponentLocation(), Direction.Rotation(), ActorSpawnParameters);
	
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(PistolFireEffect, 1.0f);
}
#pragma endregion Shooting

#pragma region Equiping And Holstering
void UCombatComponent::EquipWeapon() const
{
	if (CurrentWeapon)
	{
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		CurrentWeapon->AttachToComponent(MainPlayer->GetMesh(), AttachmentRules, FName("WeaponSocket"));
	}
}

void UCombatComponent::HolsterWeapon() const
{
	if (CurrentWeapon)
	{
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		CurrentWeapon->AttachToComponent(MainPlayer->GetMesh(), AttachmentRules, FName("PistolHolster"));
	}
}
#pragma endregion Equiping And Holstering

