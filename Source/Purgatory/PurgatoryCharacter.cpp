// Copyright Epic Games, Inc. All Rights Reserved.

#include "PurgatoryCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// APurgatoryCharacter

APurgatoryCharacter::APurgatoryCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

}

UCameraComponent* APurgatoryCharacter::GetCamera()
{
	if (FirstPersonCameraComponent != nullptr)
	{
		return FirstPersonCameraComponent;
	}
	return nullptr;
}

void APurgatoryCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

//////////////////////////////////////////////////////////////////////////
// Input

void APurgatoryCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//PlayerInputComponent->BindAction("E", IE_Pressed, this, &APurgatoryCharacter::HoldObject);
//PlayerInputComponent->BindAction("E", IE_Released, this, &APurgatoryCharacter::HoldObject);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APurgatoryCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APurgatoryCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APurgatoryCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APurgatoryCharacter::LookUpAtRate);
}

void APurgatoryCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APurgatoryCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APurgatoryCharacter::TurnAtRate(float Rate)
{
	if(Rate != 0.0f)
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void APurgatoryCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

FMatrix APurgatoryCharacter::GetCameraProjectionMatrix()
{
	FMatrix ProjectionMatrix;

	if (GetWorld()->GetFirstPlayerController()->GetLocalPlayer() != nullptr)
	{
		FSceneViewProjectionData PlayerProjectionData;

		//Will most likely crash / throw error or just break in general but i hope it works, leaving this for future reference
		GetWorld()->GetFirstLocalPlayerFromController()->GetProjectionData(GetWorld()->GetFirstPlayerController()->GetLocalPlayer()->ViewportClient->Viewport,
			EStereoscopicPass::eSSP_FULL, PlayerProjectionData);

		ProjectionMatrix = PlayerProjectionData.ProjectionMatrix;
	}
	
	return ProjectionMatrix;
}

bool APurgatoryCharacter::TraceForObjects(FHitResult hitResult, FCollisionQueryParams params)
{
	FVector EndVector = FVector((GetActorLocation().X + 50.0f), GetActorLocation().Y, GetActorLocation().Z);

	if (GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), EndVector, ECC_Pawn, params))
	{
		return true;
	}

	return false;
}
