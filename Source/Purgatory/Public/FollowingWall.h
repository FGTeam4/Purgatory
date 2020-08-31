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
	FVector PlayerFacingStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Stop;

	UFUNCTION(BlueprintCallable)
	bool CheckMove();

	UFUNCTION(BlueprintCallable)
	void SetLocation();

	UFUNCTION(BlueprintCallable)
	void RotateSelfYaw(float Degrees);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
	FVector UpVector;

protected:

	virtual void BeginPlay() override;
};