// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "FollowingWall.h"
#include "Purgatory/PurgatoryCharacter.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
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

	void Init();

	/* Gets reference to following wall and player and adds them aswell as all portals in the scene to HiddenObjects array**/
	void HandleHiddenObjects();

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void HideObjectsInRoom(FName CurrentRoomTag);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal Rendering")
		TArray<AActor*> HiddenObjects;

	UPROPERTY(EditAnywhere, Category = "Hidden Actors")
	AFollowingWall* FollowingWall;

	UPROPERTY(EditAnywhere, Category = "Hidden Actors")
	FName IgnoreTagRoom1;

	UPROPERTY(EditAnywhere, Category = "Hidden Actors")
	FName IgnoreTagRoom2;

	UPROPERTY(EditAnywhere, Category = "Hidden Actors")
	FName IgnoreTagRoom3;

	UPROPERTY(EditAnywhere, Category = "Hidden Actors")
	FName IgnoreTagRoom4;

	UPROPERTY(EditAnywhere, Category = "Hidden Actors")
	FName IgnoreTagRoom5;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal Rendering")
	TArray<UStaticMeshComponent*> HiddenComponents;

private:
	void CreateRenderTarget(UTextureRenderTarget* NewRenderTarget, APortal* Portal);

	void DestroyRenderTarget(APortal* Portal);

	//Save a reference to the PlayerController
	void SetControllerOwner(APlayerController* NewOwner);

	APortal* UpdatePortalsInTheWorld();

	UFUNCTION(BlueprintCallable)
	void RequestTeleport(APortal* Portal, ACharacter* Player);

	void GeneratePortalTexture();

	void UpdateCapture(APortal* Portal);

	UFUNCTION(BlueprintCallable)
	void Update(float DeltaTime);

private:
	TArray<AActor*> Portals;

	//Used here to be reusable
	USceneCaptureComponent2D* SceneCaptureComponent;
	
	TArray<FName> Tags;

	UPROPERTY(transient)
	UTextureRenderTarget2D* PortalTexture;

	float UpdateDelay;

	UPROPERTY(EditAnywhere, Category = "Portal Rendering")
	float RenderDistance = 5000.f;

	UPROPERTY()
	APlayerController* ControllerOwner;

	UPROPERTY()
	APurgatoryCharacter* PlayerCharacter;

	int32 PreviousScreenSizeX;
	int32 PreviousScreenSizeY;

public:
	FVector ConvertLocationToActorSpace(FVector Location, AActor* Ref, AActor* Target);
	FRotator ConvertRotationToActorSpace(FRotator Rotation, AActor* Ref, AActor* Target);
};
