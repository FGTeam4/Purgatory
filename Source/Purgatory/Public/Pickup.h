// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickable.h"
#include "Pickup.generated.h"

UCLASS()
class PURGATORY_API APickup : public AActor, public IPickable
{
	GENERATED_BODY()

private:
	FName noCollision = "NoCollision";
	
public:	
	// Sets default values for this actor's properties
	APickup();

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUp)
		class APurgatoryCharacter* holdingTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUp)
		bool bPickedUp = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PickUp)
		FName originalCollisionProfile = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Reset)
		FTransform originalTransform;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void PickUp() override;
	UFUNCTION(BlueprintCallable)
	void Drop() override;
	UFUNCTION(BlueprintCallable)
	void Reset() override;

};
