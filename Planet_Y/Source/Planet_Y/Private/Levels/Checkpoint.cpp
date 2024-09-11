#include "Levels/Checkpoint.h"
#include "Components/SphereComponent.h"
#include "Player/MainPlayer.h"

ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	SphereCollission = CreateDefaultSubobject<USphereComponent>("Collision Sphere");
	SphereCollission->SetupAttachment(RootComponent);

	SphereCollission->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlapBegin);
}

void ACheckpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMainPlayer* Player = Cast<AMainPlayer>(OtherActor))
	{
		Player->LastCheckpoint = this;
	}
}

