#include "PickUp.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APickUp::APickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionShpere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionShpere;
	CollisionShpere->OnComponentBeginOverlap.AddDynamic(this, &APickUp::OnOverlapBegin);
	CollisionShpere->OnComponentEndOverlap.AddDynamic(this, &APickUp::OnOverlapEnd);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MyParticle"));
	Particle->SetupAttachment(GetRootComponent());

	bRotatation = false;
	RotationRate = 50.f;
}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotatation) {
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw = Rotation.Yaw + DeltaTime * RotationRate;
		SetActorRotation(Rotation);
	}

}

void APickUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Destroy();
}

void APickUp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}