#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class ABaseBullet;

UCLASS()
class PLANET_Y_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	ABaseWeapon();

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseBullet> WeaponBullet;

	UPROPERTY(EditAnywhere)
	USceneComponent* BulletSpawnPoint;

	// Weapon Properties
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRate;
	
private:	

	UPROPERTY(EditAnywhere)
	USceneComponent* Root;	

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* WeaponMesh;
};

