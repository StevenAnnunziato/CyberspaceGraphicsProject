// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Constructor
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true; // makes the object orient towards the movement vector
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	//GetCharacterMovement()->JumpZVelocity = 600.0f;
	//GetCharacterMovement()->AirControl = 0.7f;

	// Create a SpringArmComponent as a child of the RootComponent, the capsule
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // camera does not rotate relative to the arm

	mIsDead = false;
	mAirJumpsRemaining = mMaxAirJumps;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnBeginOverlap);

	mAirJumpsRemaining = mMaxAirJumps;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// bind mouse input
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnUp", this, &APawn::AddControllerPitchInput);

	// bind jumping
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::Jump);

	// bind movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
}

void APlayerCharacter::MoveForward(float axis)
{
	if (!mIsDead)
	{
		// get forward rotation
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0.0f, rotation.Yaw, 0.0f);

		// calculate forward direction from the yaw vector
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, axis);
	}
}

void APlayerCharacter::MoveRight(float axis)
{
	if (!mIsDead)
	{
		// get forward rotation
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0.0f, rotation.Yaw, 0.0f);

		// calculate forward direction from the yaw vector
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(direction, axis);
	}
}

void APlayerCharacter::Jump()
{
	//Super::Jump();
	// if you are in the air
	if (GetMovementComponent()->IsFalling())
	{
		if (mAirJumpsRemaining > 0)
		{
			OnDoubleJump();
			Super::LaunchCharacter(FVector(0.0f, 0.0f, GetCharacterMovement()->JumpZVelocity), false, true);
			mAirJumpsRemaining--;
		}
	}
	else // on the ground
	{
		Super::LaunchCharacter(FVector(0.0f, 0.0f, GetCharacterMovement()->JumpZVelocity), false, true);
	}
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	// reset air jumps
	mAirJumpsRemaining = mMaxAirJumps;
}

void APlayerCharacter::OnBeginOverlap(UPrimitiveComponent* pHitComp, AActor* pOtherActor,
                                      UPrimitiveComponent* pOtherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	
	if (pOtherActor->ActorHasTag("Enemy"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Hit Enemy"));

		// die
		if (!mIsDead)
		{
			mIsDead = true;

			// ragdoll
			GetMesh()->SetSimulatePhysics(true);

			// restart after a few seconds
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayerCharacter::RestartGame, 3.0f, false);
		}
		
	}
	else if (pOtherActor->ActorHasTag("Pickup"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Hit Pickup"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Hit Unknown"));
	}
}

void APlayerCharacter::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
