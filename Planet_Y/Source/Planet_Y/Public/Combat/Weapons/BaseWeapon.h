#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class PLANET_Y_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	ABaseWeapon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:	

	UPROPERTY(EditAnywhere)
	USceneComponent* Root;	

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* WeaponMesh;
};
