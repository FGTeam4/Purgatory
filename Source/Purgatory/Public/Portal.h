#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
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
	USceneComponent* PortalRootComponent;
	UStaticMeshComponent* PortalRenderComponent;

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

	bool IsActive();

	void SetActive(bool NewActive);

	UFUNCTION(BlueprintCallable)
		void TeleportPlayer();

	bool IsPlayerInFrontOfPortal(FVector point, FVector PortalLocation, FVector PortalNormal);

	bool IsPlayerCrossingPortal(FVector Point, FVector PortalLocaion, FVector PortalNormal);

	/* Render texture functions */
	UFUNCTION(BlueprintImplementableEvent, Category = "Portal")
		void ClearRTT();

	UFUNCTION(BlueprintImplementableEvent, Category = "Portal")
		void SetRTT(UTexture* RenderTexture);

	UFUNCTION(BlueprintImplementableEvent, Category = "Portal")
		void ForceTick();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsActive = true;

		ACharacter* player;
private:

	AActor* TargetObject = nullptr;

public:
	/* PURELY HELPER FUNCTIONS */
	FVector ConvertLocationToActorSpace(FVector Location, AActor* Ref, AActor* Target);
	FRotator ConvertRotationToActorSpace(FRotator Rotation, AActor* Ref, AActor* Target);
};