// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"

// Sets default values
APortalManager::APortalManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	PortalTexture = nullptr;
	UpdateDelay = 1.1f;

	PreviousScreenSizeX = 0;
	PreviousScreenSizeY = 0;
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
	//GeneratePortalTexture();
}

void APortalManager::UpdateCapture(APortal* Portal)
{
}

void APortalManager::Update(float DeltaTime)
{
	UpdateDelay += DeltaTime;

	if (UpdateDelay > 1.0f)
	{
		UpdateDelay = 0.0f;
		GeneratePortalTexture();
	}

	//APortal* Portal = UpdatePortalsInTheWorld();
	//
	//if (Portal != nullptr)
	//{
	//	UpdateCapture(Portal);
	//}
}

// Called when the game starts or when spawned
void APortalManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APortalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortalManager::CreateRenderTarget(UTextureRenderTarget* NewRenderTarget, APortal* Portal)
{

}

void APortalManager::DestroyRenderTarget(APortal* Portal)
{
}

APortal* APortalManager::UpdatePortalsInTheWorld()
{
	if (ControllerOwner == nullptr)
	{
		return nullptr;
	}

	ACharacter* Character = ControllerOwner->GetCharacter();

	APortal* ActivePortal = nullptr;
	FVector PlayerLocation = Character->GetActorLocation();
	//FVector CameraLocation = Character->GetCameraComponent()->GetComponentLocation();
	
	return nullptr;
}

bool APortalManager::RequestTeleport(APortal* Portal, ACharacter* Player)
{
	if (!Portal->bIsActive)
	{
		return false;
	}

	return true;
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

		//PortalTexture->nAutoGenerateMips = false;

		//Force the engine to create a new render target
		PortalTexture->UpdateResource();
	}

	else
	{
		PortalTexture->ResizeTarget(CurrentSizeX, CurrentSizeY);
	}
}

