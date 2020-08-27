// Fill out your copyright notice in the Description page of Project Settings.
#include "Pickup.h"
#include "Purgatory/PurgatoryCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	originalCollisionProfile = mesh->GetCollisionProfileName();
	originalTransform = mesh->GetComponentTransform();
	
	ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	holdingTarget = (APurgatoryCharacter*)character;
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickup::PickUp()
{
	if (bPickedUp == false)
	{
		bPickedUp = true;
		mesh->SetSimulatePhysics(false);
		mesh->SetCollisionProfileName(noCollision);
		AttachToActor(holdingTarget,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void APickup::Drop()
{
	if (bPickedUp == true)
	{
		bPickedUp = false;
		mesh->SetCollisionProfileName(originalCollisionProfile);
		mesh->SetSimulatePhysics(true);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void APickup::Reset()
{
	bPickedUp = false;
	mesh->SetCollisionProfileName(originalCollisionProfile);
	mesh->SetSimulatePhysics(true);
	mesh->SetWorldTransform(originalTransform);
}

