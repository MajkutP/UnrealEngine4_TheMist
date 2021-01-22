#include "ItemPickUp.h"
#include "KnightCharacter.h"

AItemPickUp::AItemPickUp(){
	bRotatation = true;
	ManaAmountToRestore = 10.0f;
}

void AItemPickUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor) {
		AKnightCharacter* Knight = Cast<AKnightCharacter>(OtherActor);
		if (Knight) {
			Knight->IncreaseMana(ManaAmountToRestore);
		}
	}
}

void AItemPickUp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}