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
	void ResetWallOnRestart();

	UFUNCTION(BlueprintCallable)
	void SetYawRotation(float Degrees);

	/**
	* Stops the MoveActor timer
	*/
	UFUNCTION(BlueprintCallable)
	void StopMoveActorTimer();

public:

	/**
	* The distance the Following Wall shall keep to the player.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "200", ClampMax = "1000"))
	float DistanceToPlayer = 500.0f;

	/**
	* Time interval between movement changes. 
	* Higher interval means slower wall movement.
	*/
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.00001", ClampMax = "1"))
	float MoveActorInterval = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartingRotation = -90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RestartOffsetMultiplier = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlayerFacingStart = FVector(1.0f, 0.0f, 0.0f);

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
	* Calculate where to move Wall
	*/
	void CalculateLocation();

	/**
	* Starts the MoveActor timer
	*/
	void StartMoveActorTimer();

	/**
	* Check distance between this Wall and the Player
	*/
	bool CheckDistance();

	/**
	* Move the Wall towards the Player
	*/
	void MoveActor();

private:

	UPROPERTY()
	ACharacter* PlayerCharacter;

	/**
	* Timer Handle for Move Timer
	*/
	FTimerHandle MoveActorTimerHandle;

	UPROPERTY()
	FVector MoveVector;

	UPROPERTY()
	FVector ActorLocation;

	UPROPERTY()
	FVector CurrentPlayerLocation;

	UPROPERTY()
	FVector StartPosition;

	UPROPERTY()
	FVector PlayerFacingCurrent;

	UPROPERTY()
	float MoveAmount = 0.01f;

	UPROPERTY()
	int MaxMoveStep = 100;

	UPROPERTY()
	int CurrentMoveStep = 0;

};