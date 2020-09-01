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
	void MoveActor();

	UFUNCTION(BlueprintCallable)
	void RotateActorYaw(float Degrees);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceToPlayer = 400.0f;

	UPROPERTY(EditAnywhere)
	float MoveAmount = 0.0001f;

	UPROPERTY(EditAnywhere)
	int MoveSpeed = 10;

protected:

	virtual void BeginPlay() override;

private:

	UFUNCTION()
	bool CheckDistance();

	UFUNCTION()
	void CalculateLocation();

private:

	UPROPERTY()
	FVector MoveVector;

	UPROPERTY()
	FVector PlayerFacingStart;

	UPROPERTY()
	bool bMoving;

	UPROPERTY()
	int CurrentMoveStep = 0;

};