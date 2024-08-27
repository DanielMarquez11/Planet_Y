#include "Combat/Weapons/BaseWeapon.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetRootComponent());

	BulletSpawnPoint = CreateDefaultSubobject<USceneComponent>("BulletSpawnPoint");
	BulletSpawnPoint->SetupAttachment(WeaponMesh);
}

