#include "Levels/Checkpoint.h"
#include "Components/SphereComponent.h"

ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	SphereCollission = CreateDefaultSubobject<USphereComponent>("Collision Sphere");
	SphereCollission->SetupAttachment(RootComponent);
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
}

void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

