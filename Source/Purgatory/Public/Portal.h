// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	bool LastInFront;
protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsActive();

	void SetActive(bool NewActive);

	void TeleportPlayer();
	
	bool IsPlayerInFrontOfPortal(FVector point, FVector PortalLocation, FVector PortalNormal);

	bool IsPlayerCrossingPortal(FVector Point, FVector PortalLocaion, FVector PortalNormal);

public:
	bool bIsActive = true;

	//Linked point of the portal
	AActor* target;

<<<<<<< Updated upstream
	APurgatoryCharacter* player;
=======
	UPROPERTY(EditAnywhere, Category = "Portal")
		ACharacter* player;

public:
	/* PURELY HELPER FUNCTIONS */
	FVector ConvertLocationToActorSpace(FVector Location, AActor* Ref, AActor* Target);
	FRotator ConvertRotationToActorSpace(FRotator Rotation, AActor* Ref, AActor* Target);
>>>>>>> Stashed changes
};
