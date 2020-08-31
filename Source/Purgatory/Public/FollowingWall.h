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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DistanceFirst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DistanceSecond;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DistanceThird;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DistanceFourth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceToPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector NewLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlayerFacingStart;

	UFUNCTION(BlueprintCallable)
	bool CheckDistance();

	UFUNCTION(BlueprintCallable)
	FVector CalculateLocation();

	UFUNCTION(BlueprintCallable)
	void MoveActor(FVector Destination);

	UFUNCTION(BlueprintCallable)
	void RotateActorYaw(float Degrees);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
	FVector UpVector;

protected:

	virtual void BeginPlay() override;
};