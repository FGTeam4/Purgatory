// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Purgatory\PurgatoryCharacter.h"
#include "FollowingWall.generated.h"

UCLASS()
class PURGATORY_API AFollowingWall : public AActor
{
	GENERATED_BODY()

public:

	AFollowingWall();

	UPROPERTY(EditAnywhere)
	ACharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere)
	float Distance;

	UPROPERTY(EditAnywhere)
	FVector PlayerFacingStart;

	UFUNCTION(BlueprintCallable)
	void SetLocation();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

protected:

	virtual void BeginPlay() override;
};