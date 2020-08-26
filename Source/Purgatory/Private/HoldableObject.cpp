// Fill out your copyright notice in the Description page of Project Settings.


#include "HoldableObject.h"

// Sets default values
AHoldableObject::AHoldableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AHoldableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHoldableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

