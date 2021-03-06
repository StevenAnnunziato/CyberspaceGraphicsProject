// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class CYBERSPACEGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;

	void MoveForward(float axis);
	void MoveRight(float axis);
	void Jump();
	void Landed(const FHitResult& Hit) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Settings")
		int mMaxAirJumps;
	int mAirJumpsRemaining;
	bool mIsDead;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnDoubleJump();

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* pHitComp, class AActor* pOtherActor,
			class UPrimitiveComponent* pOtherComponent, int32 otherBodyIndex, bool bFromSweep,
			const FHitResult &sweepResult);

	void RestartGame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
