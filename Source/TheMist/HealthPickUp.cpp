#include "HealthPickUp.h"
#include "KnightCharacter.h"

AHealthPickUp::AHealthPickUp() {
	bRotatation = true;
	HealthAmountToRestore = 30.f;
}

void AHealthPickUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor) {
		AKnightCharacter* Knight = Cast<AKnightCharacter>(OtherActor);
		if (Knight) {
			Knight->IncreaseHealth(HealthAmountToRestore);
		}
		Destroy();
	}
}

void AHealthPickUp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
