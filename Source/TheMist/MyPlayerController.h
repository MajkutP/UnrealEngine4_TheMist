// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class THEMIST_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UUserWidget> HUDAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	UUserWidget* HUDOverlay;

	virtual void BeginPlay() override;
};
