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

	/**
	* Calculate where to move Wall
	*/
	UFUNCTION(BlueprintCallable)
	void CalculateLocation();

	/**
	* Starts the MoveActor timer
	*/
	UFUNCTION(BlueprintCallable)
	void StartMoveActorTimer();

	/**
	* Stops the MoveActor timer
	*/
	UFUNCTION(BlueprintCallable)
	void StopMoveActorTimer();

public:

	/**
	* The distance the Following Wall shall keep to the player.
	*/
	UPROPERTY(EditAnywhere, meta = (ClampMin = "200", ClampMax = "1000"))
	float DistanceToPlayer = 500.0f;

	/**
	* Time interval between movement changes. 
	* Higher interval means slower wall movement.
	*/
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.00001", ClampMax = "1"))
	float MoveActorInterval = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlayerFacingStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DistanceFirst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DistanceSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DistanceThird;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DistanceFourth;

private:

	/**
	* Check distance between this Wall and the Player
	*/
	bool CheckDistance();

	/**
	* Move the Wall towards the Player
	*/
	void MoveActor();

private:

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
	float MoveAmount = 0.01f;

	UPROPERTY()
	int MoveSpeed = 100;

	UPROPERTY()
	int CurrentMoveStep = 0;

};