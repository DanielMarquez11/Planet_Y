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

	void StartJump();
	void StopJump();

	void Dash();
	
private:

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UAnimMontage* DoubleJumpMontage;
};
