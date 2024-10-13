#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBullet.generated.h"

class UCapsuleComponent;
class UProjectileMovementComponent;

UCLASS()
class PLANET_Y_API ABaseBullet : public AActor
{
	GENERATED_BODY()
	
public:	

	ABaseBullet();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	
	void DestroyBullet();

	// Bullet Components
	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	UCapsuleComponent* CollisionCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Bullet Properties
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BulletSpeed = 15000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float LifeTime = 2.0f;
};
