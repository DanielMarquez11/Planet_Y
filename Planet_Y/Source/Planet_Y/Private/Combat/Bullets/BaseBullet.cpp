#include "Combat/Bullets/BaseBullet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/Damageable.h"

ABaseBullet::ABaseBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	BulletMesh->SetupAttachment(GetRootComponent());
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>("CapsuleCollision");
	CollisionCapsule->SetupAttachment(GetRootComponent());
	
	CollisionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionCapsule->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionCapsule->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CollisionCapsule->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

	ProjectileMovementComponent->InitialSpeed = BulletSpeed;
	ProjectileMovementComponent->MaxSpeed = BulletSpeed;

	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void ABaseBullet::BeginPlay()
{
	Super::BeginPlay();

	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ABaseBullet::OnCapsuleBeginOverlap);

	FTimerHandle DestructionTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DestructionTimerHandle, this, &ABaseBullet::DestroyBullet, LifeTime, false);
}

void ABaseBullet::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		Cast<IDamageable>(OtherActor)->Execute_TakeDamageToHealth(OtherActor, 25.0f);
	}

	DestroyBullet();
}


void ABaseBullet::DestroyBullet()
{
	Destroy();
}

