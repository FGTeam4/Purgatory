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

	UFUNCTION(BlueprintCallable)
	void ResetWall(FVector PlayerSpawnLocation);

	UFUNCTION(BlueprintCallable)
	void SetYawRotation(float Degrees);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "500"))
	float DistanceToPlayer = 300.0f;

	UPROPERTY(EditAnywhere)
	float MoveAmount = 0.01f;

	UPROPERTY(EditAnywhere)
	int MoveSpeed = 100;

	/**
	* Time interval between movement changes
	*/
	UPROPERTY(EditAnywhere)
	float MoveActorInterval = 0.01f;

	UPROPERTY(EditAnywhere)
	ACharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DistanceFirst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DistanceSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DistanceThird;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DistanceFourth;

protected:

	virtual void BeginPlay() override;

private:

	/**
	* Check distance between this Wall and the Player
	*/
	bool CheckDistance();

	/**
	* Move the Wall towards the Player
	*/
	void MoveActor();

	/**
	* Calculate where to move Wall
	*/
	void CalculateLocation();

	/**
	* Starts the MoveActor timer
	*/
	void StartMoveActorTimer();

	/**
	* Stops the MoveActor timer
	*/
	void StopMoveActorTimer();

private:

	/**
	* Timer Handle for Move Timer
	*/
	FTimerHandle MoveActorTimerHandle;

	UPROPERTY()
	FVector MoveVector;

	UPROPERTY()
	FVector PlayerFacingStart;

	UPROPERTY()
	FVector ActorLocation;

	UPROPERTY()
	FVector CurrentPlayerLocation;

	UPROPERTY()
	int CurrentMoveStep = 0;

};