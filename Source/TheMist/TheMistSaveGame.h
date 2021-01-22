// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TheMistSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStatistics {
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, Category = "Data")
		float Health;
		UPROPERTY(VisibleAnywhere, Category = "Data")
		float MaxHealth;
		UPROPERTY(VisibleAnywhere, Category = "Data")
		float Stamina;
		UPROPERTY(VisibleAnywhere, Category = "Data")
		float MaxStamina;
		UPROPERTY(VisibleAnywhere, Category = "Data")
		float Mana;
		UPROPERTY(VisibleAnywhere, Category = "Data")
		float MaxMana;
		UPROPERTY(VisibleAnywhere, Category = "Data")
		FRotator Rotation;
		UPROPERTY(VisibleAnywhere, Category = "Data")
		FVector Location;
};

/**
 * 
 */
UCLASS()
class THEMIST_API UTheMistSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UTheMistSaveGame();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString CharacterName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FCharacterStatistics CharStats;
	
};
