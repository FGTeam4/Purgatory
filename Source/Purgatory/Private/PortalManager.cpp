// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"

// Sets default values
APortalManager::APortalManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	PortalTexture = nullptr;
	UpdateDelay = 1.1f;

	PreviousScreenSizeX = 0;
	PreviousScreenSizeY = 0;
}

// Called when the game starts or when spawned
void APortalManager::BeginPlay()
{
	Super::BeginPlay();
	this->AttachToActor(PlayerCharacter, FAttachmentTransformRules::SnapToTargetIncludingScale);
	this->Init();
}

// Called every frame
void APortalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Update(DeltaTime);
}


void APortalManager::Init()
{
	SceneCaptureComponent = NewObject<USceneCaptureComponent2D>(this, USceneCaptureComponent2D::StaticClass(), *FString("PortalSceneCaptureComponent"));
	SceneCaptureComponent->RegisterComponent();

	//Mostly post processing
	SceneCaptureComponent->bCaptureEveryFrame = false;
	SceneCaptureComponent->bCaptureOnMovement = false;
	SceneCaptureComponent->LODDistanceFactor = 3;
	SceneCaptureComponent->TextureTarget = nullptr;
	SceneCaptureComponent->bEnableClipPlane = true;
	SceneCaptureComponent->bUseCustomProjectionMatrix = true;
	//Expensive, but needed to render all information.
	SceneCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;

	FPostProcessSettings CaptureSettings;

	CaptureSettings.bOverride_AmbientOcclusionQuality = true;
	CaptureSettings.bOverride_MotionBlurAmount = true;
	CaptureSettings.bOverride_SceneFringeIntensity = true;
	CaptureSettings.bOverride_GrainIntensity = true;
	CaptureSettings.bOverride_ScreenSpaceReflectionQuality = true;

	CaptureSettings.AmbientOcclusionQuality = 0.0f;
	CaptureSettings.MotionBlurAmount = 0.0f;
	CaptureSettings.SceneFringeIntensity = 0.0f;
	CaptureSettings.GrainIntensity = 0.0f;
	CaptureSettings.ScreenSpaceReflectionQuality = 0.0f;

	CaptureSettings.bOverride_ScreenPercentage = true;
	CaptureSettings.ScreenPercentage = 100.0f;

	SceneCaptureComponent->PostProcessSettings = CaptureSettings;

	//Create RTT Buffer
	GeneratePortalTexture();
}

void APortalManager::UpdateCapture(APortal* Portal)
{
	//Retrieve Reference to character and player controller (Maybe just character though?)
	if (PlayerCharacter == nullptr)
	{
		return;
	}

	//Update SceneCapture

	if (SceneCaptureComponent != nullptr && PortalTexture != nullptr
		&& Portal != nullptr && PlayerCharacter != nullptr)
	{
		UCameraComponent* PlayerCamera = PlayerCharacter->GetCamera();
		AActor* Target = Portal->GetTarget();

		if (Target != nullptr)
		{
			//Compute new location in the space of target actor(May not be aligned in world)
			FVector NewLocation = ConvertLocationToActorSpace(PlayerCamera->GetComponentLocation(), Portal, Target);
			SceneCaptureComponent->SetWorldLocation(NewLocation);

			//Compute new rotation in the space of target location
			FTransform CameraTransform = PlayerCamera->GetComponentTransform();
			FTransform SourceTransform = Portal->GetActorTransform();
			FTransform TargetTransform = Target->GetActorTransform();

			FQuat LocalQuat = SourceTransform.GetRotation().Inverse() * CameraTransform.GetRotation();
			FQuat NewWorldQuat = TargetTransform.GetRotation() * LocalQuat;

			//Update SceneCapture Rotation
			SceneCaptureComponent->SetWorldRotation(NewWorldQuat);

			//Clip Plane to ignore objects between the scene capture and the target of the portal
			SceneCaptureComponent->ClipPlaneNormal = Target->GetActorForwardVector();
			SceneCaptureComponent->ClipPlaneBase = Target->GetActorLocation() + 
				(SceneCaptureComponent->ClipPlaneNormal * -1.5f); //Offset to avoid visible pixel border
		}

		Portal->SetActive(true);

		//Assign render target
		Portal->SetRTT(PortalTexture);
		SceneCaptureComponent->TextureTarget = PortalTexture;

		//Get the projection matrix
		SceneCaptureComponent->CustomProjectionMatrix = PlayerCharacter->GetCameraProjectionMatrix();

		//Render scene from this Component for the portals
		SceneCaptureComponent->CaptureScene();
	}
}

void APortalManager::Update(float DeltaTime)
{
	UpdateDelay += DeltaTime;

	if (UpdateDelay > 1.0f)
	{
		UpdateDelay = 0.0f;
		GeneratePortalTexture();
	}

	APortal* Portal = UpdatePortalsInTheWorld();
	
	if (Portal != nullptr)
	{
		UpdateCapture(Portal);
	}
}

void APortalManager::CreateRenderTarget(UTextureRenderTarget* NewRenderTarget, APortal* Portal)
{
	//Implementation in blueprint
}

void APortalManager::DestroyRenderTarget(APortal* Portal)
{
	//Implementation in blueprint
}

APortal* APortalManager::UpdatePortalsInTheWorld()
{
	if (PlayerCharacter == nullptr)
	{
		return nullptr;
	}

	//ACharacter* Character = ControllerOwner->GetCharacter();
	ACharacter* Character = PlayerCharacter;

	APortal* ActivePortal = nullptr;
	FVector PlayerLocation = Character->GetActorLocation();
	FVector CameraLocation = PlayerCharacter->GetCamera()->GetComponentLocation();
	
	float Distance = 4096.0f;

	for (TActorIterator<APortal>ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		APortal* Portal = *ActorItr;
		FVector PortalLocation = Portal->GetActorLocation();
		FVector PortalNormal = -1 * Portal->GetActorForwardVector();

		//Reset portal
		Portal->ClearRTT();
		Portal->SetActive(false);

		float NewDistance = FMath::Abs(FVector::Dist(PlayerLocation, PortalLocation));

		if (NewDistance < Distance)
		{
			Distance = NewDistance;
			ActivePortal = Portal;
		}
	}

	return ActivePortal;
}

void APortalManager::RequestTeleport(APortal* Portal, ACharacter* Player)
{
	if (Portal != nullptr && Player != nullptr)
	{
		Portal->TeleportPlayer(Player);

		APortal* FuturePortal = UpdatePortalsInTheWorld();

		if (FuturePortal != nullptr)
		{
			FuturePortal->ForceTick();
			UpdateCapture(FuturePortal);
		}
	}
}

void APortalManager::GeneratePortalTexture()
{
	int32 CurrentSizeX = 1920;
	int32 CurrentSizeY = 1080;

	if (ControllerOwner != nullptr)
	{
		ControllerOwner->GetViewportSize(CurrentSizeX, CurrentSizeY);
	}

	//Use smaller size than screen to reduce impact
	CurrentSizeX = FMath::Clamp(int(CurrentSizeX / 1.7), 128, 1920);
	CurrentSizeY = FMath::Clamp(int(CurrentSizeY / 1.7), 128, 1080);

	if (CurrentSizeX == PreviousScreenSizeX && CurrentSizeY == PreviousScreenSizeY)
	{
		return;
	}

	PreviousScreenSizeX = CurrentSizeX;
	PreviousScreenSizeY = CurrentSizeY;

	//If no render target exists, create one
	if (PortalTexture == nullptr)
	{
		//Create new RTT
		PortalTexture = NewObject<UTextureRenderTarget2D>(this, UTextureRenderTarget2D::StaticClass(), *FString("PortalRenderTarget"));
		check(PortalTexture);

		PortalTexture->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f;
		PortalTexture->Filter = TextureFilter::TF_Bilinear;
		PortalTexture->SizeX = CurrentSizeX;
		PortalTexture->SizeY = CurrentSizeY;
		PortalTexture->ClearColor = FLinearColor::Black;
		PortalTexture->TargetGamma = 2.2f;
		PortalTexture->bNeedsTwoCopies = false;
		PortalTexture->AddressX = TextureAddress::TA_Clamp;
		PortalTexture->AddressY = TextureAddress::TA_Clamp;

		//PortalTexture->bAutoGenerateMips = false;

		//Force the engine to create a new render target
		PortalTexture->UpdateResource();
	}

	else
	{
		PortalTexture->ResizeTarget(CurrentSizeX, CurrentSizeY);
	}
}

FVector APortalManager::ConvertLocationToActorSpace(FVector Location, AActor* Ref, AActor* Target)
{
	if (Ref == nullptr || Target == nullptr)
	{
		return FVector::ZeroVector;
	}

	FVector Direction = Location - Ref->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();

	FVector Dots;
	Dots.X = FVector::DotProduct(Direction, Ref->GetActorForwardVector());
	Dots.Y = FVector::DotProduct(Direction, Ref->GetActorRightVector());
	Dots.Z = FVector::DotProduct(Direction, Ref->GetActorUpVector());

	FVector NewDirection = Dots.X * Target->GetActorForwardVector()
		+ Dots.Y * Target->GetActorRightVector()
		+ Dots.Z * Target->GetActorUpVector();

	return TargetLocation + NewDirection;
}

FRotator APortalManager::ConvertRotationToActorSpace(FRotator Rotation, AActor* Ref, AActor* Target)
{
	if (Ref == nullptr || Target == nullptr)
	{
		return FRotator::ZeroRotator;
	}

	FTransform SourceTransform = Ref->GetActorTransform();
	FTransform TargetTransform = Target->GetActorTransform();

	FQuat QuatRotation = FQuat(Rotation);

	FQuat LocalQuat = SourceTransform.GetRotation().Inverse() * QuatRotation;
	FQuat NewWorldQuat = TargetTransform.GetRotation() * LocalQuat;

	return NewWorldQuat.Rotator();
}