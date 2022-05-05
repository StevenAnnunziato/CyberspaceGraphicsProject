// Fill out your copyright notice in the Description page of Project Settings.


#include "Cyberspace_GameMode.h"
#include "GameFramework/Actor.h"


ACyberspace_GameMode::ACyberspace_GameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACyberspace_GameMode::BeginPlay()
{
	Super::BeginPlay();

	// spawn an enemy every few seconds
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ACyberspace_GameMode::SpawnEnemy, FMath::RandRange(1.0f, 3.5f), true);
}

void ACyberspace_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACyberspace_GameMode::SpawnEnemy()
{
	float randX = FMath::RandRange(SpawnXBounds.X, SpawnXBounds.Y);
	float randY = FMath::RandRange(SpawnYBounds.X, SpawnYBounds.Y);
	float randZ = FMath::RandRange(SpawnZBounds.X, SpawnZBounds.Y);

	FVector SpawnPosition = FVector(randX, randY, randZ);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	GetWorld()->SpawnActor(Enemy, &SpawnPosition, &SpawnRotation);
}
