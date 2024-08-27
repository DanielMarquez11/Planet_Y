#include "Combat/Bullets/BaseBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABaseBullet::ABaseBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	BulletMesh->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

	ProjectileMovementComponent->InitialSpeed = BulletSpeed;
	ProjectileMovementComponent->MaxSpeed = BulletSpeed;

	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void ABaseBullet::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle DestructionTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DestructionTimerHandle, this, &ABaseBullet::DestroyBullet, LifeTime, false);
}

void ABaseBullet::DestroyBullet()
{
	Destroy();
}

