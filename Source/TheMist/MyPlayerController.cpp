#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMyPlayerController::BeginPlay() {
	Super::BeginPlay();

	if (HUDAsset) {
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDAsset);
	}

	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);
}