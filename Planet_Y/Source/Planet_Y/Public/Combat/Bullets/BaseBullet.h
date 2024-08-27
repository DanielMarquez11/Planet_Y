#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBullet.generated.h"

class UProjectileMovementComponent;

UCLASS()
class PLANET_Y_API ABaseBullet : public AActor
{
	GENERATED_BODY()
	
public:	

	ABaseBullet();

	virtual void BeginPlay() override;

private:

	void DestroyBullet();

	// Bullet Components
	UPROPERTY(EditAnywhere, Category = "Bullet")
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, Category = "Bullet")
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Bullet Properties
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BulletSpeed = 8000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float LifeTime = 2.0f;
};
