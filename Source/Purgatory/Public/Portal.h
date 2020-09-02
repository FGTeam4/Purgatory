// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime\Engine\Classes\Engine\TriggerBox.h>
#include "Purgatory\PurgatoryCharacter.h"
#include "Portal.generated.h"

UCLASS()
class PURGATORY_API APortal : public AActor
{
	GENERATED_BODY()

public:
	APortal();

public:
	ATriggerBox* PortalTrigger;

private:

	FVector LastPosition;
	bool bLastInFront;
	FVector TargetTeleportLocation;
protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Portal")
	AActor* GetTarget();

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void SetTarget(AActor* NewTarget);

	UFUNCTION(BlueprintPure)
	bool IsActive();

	void SetActive(bool NewActive);

	UFUNCTION(BlueprintCallable)
	void TeleportPlayer(AActor* ActorToTeleport);

	/* Render texture functions */
	UFUNCTION(BlueprintImplementableEvent, Category = "Portal")
	void ClearRTT();

	UFUNCTION(BlueprintImplementableEvent, Category = "Portal")
	void SetRTT(UTexture* RenderTexture);

	UFUNCTION(BlueprintImplementableEvent, Category = "Portal")
	void ForceTick();

public:

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* PortalRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActive = false;

	UPROPERTY(BlueprintReadWrite, Category = "Portal")
	ACharacter* Player;

private:
	AActor* TargetObject = nullptr;

private :
	/* PURELY HELPER FUNCTIONS */
	FVector ConvertLocationToActorSpace(FVector Location, AActor* Ref, AActor* Target);
	FRotator ConvertRotationToActorSpace(FRotator Rotation, AActor* Ref, AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Portal")
	bool IsPlayerInFrontOfPortal(FVector point, FVector PortalLocation, FVector PortalNormal);

	UFUNCTION(BlueprintCallable, Category = "Portal")
	bool IsPlayerCrossingPortal(FVector Point, FVector PortalLocaion, FVector PortalNormal);

	UFUNCTION(BlueprintCallable, Category = "Portal")
	bool IsPlayerBehindPortal(FVector Point, FVector PortalLocation, FVector PortalNormal);

	UFUNCTION(BlueprintCallable)
	bool IsPointInsideBox(FVector Point, UBoxComponent* Box);

	UFUNCTION(BlueprintCallable)
	AActor* GetPortalManager(AActor* Context);
};