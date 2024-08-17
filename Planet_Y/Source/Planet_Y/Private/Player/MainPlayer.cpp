#include "Player/MainPlayer.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMainPlayer::AMainPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Character Rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false; // Makes sure the player doesnt rotate when the camera rotates
	bUseControllerRotationRoll = false;

	// Capsule Component
	GetCapsuleComponent()->InitCapsuleSize(35.f, 60.0f);

	// Jumping
	JumpMaxHoldTime = 0.1f;
	JumpMaxCount = 2;
	
	// Movement Component
	GetCharacterMovement()->bOrientRotationToMovement = true; // Makes the player rotate to the way its moving
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f); // How fast the player rotates
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	GetCharacterMovement()->GravityScale = 2.5f;
	GetCharacterMovement()->MaxAcceleration = 2300.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;

	GetCharacterMovement()->MaxWalkSpeed = 850.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 4000.0f;

	GetCharacterMovement()->JumpZVelocity = 800.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 100.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	// Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = 450.0f; // Range of camera
	CameraBoom->SocketOffset =FVector(0, 0, 40.0f);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

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
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMainPlayer::StartJump()
{
	if (JumpCurrentCount == 1)
	{
		const FVector PlayerVelocity = (GetCharacterMovement()->Velocity / 10) * 4.0f;
		GetCharacterMovement()->Velocity = PlayerVelocity + (GetLastMovementInputVector() * 680.0f);
		
		Jump();
	}
	else
	{
		Jump();
	}
}

void AMainPlayer::StopJump()
{
	StopJumping();
}

void AMainPlayer::Dash()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, "Dash");
}


