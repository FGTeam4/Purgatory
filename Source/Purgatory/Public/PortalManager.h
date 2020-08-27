// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "Portal.h"
#include "Engine/TextureRenderTarget.h"
#include "PortalManager.generated.h"

UCLASS()
class PURGATORY_API APortalManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalManager(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void CreateRenderTarget(UTextureRenderTarget* NewRenderTarget, APortal* Portal);

	void DestroyRenderTarget(APortal* Portal);

	//Save a reference to the PlayerController
	void SetControllerOwner(APlayerController* NewOwner);

	APortal* UpdatePortalsInTheWorld();

	bool RequestTeleport(APortal* Portal, ACharacter* Player);

	void GeneratePortalTexture();

	void Init();

	void UpdateCapture(APortal* Portal);

	void Update(float DeltaTime);

private:
	TArray<AActor*> Portals;

	//Used here to be reusable
	USceneCaptureComponent2D* SceneCaptureComponent;

	UPROPERTY(transient)
	UTextureRenderTarget2D* PortalTexture;

	float UpdateDelay;

	UPROPERTY()
	APlayerController* ControllerOwner;

	int32 PreviousScreenSizeX;
	int32 PreviousScreenSizeY;
};
