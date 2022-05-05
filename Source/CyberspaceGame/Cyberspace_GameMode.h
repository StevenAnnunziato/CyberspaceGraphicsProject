// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Cyberspace_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSPACEGAME_API ACyberspace_GameMode : public AGameMode
{
	GENERATED_BODY()

	ACyberspace_GameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemy();

	UPROPERTY(EditAnywhere, Category = "SpawnSettings")
		TSubclassOf<APawn> Enemy;
	
	UPROPERTY(EditAnywhere, Category = "SpawnSettings")
		FVector2D SpawnXBounds;
	UPROPERTY(EditAnywhere, Category = "SpawnSettings")
		FVector2D SpawnYBounds;
	UPROPERTY(EditAnywhere, Category = "SpawnSettings")
		FVector2D SpawnZBounds;
};
