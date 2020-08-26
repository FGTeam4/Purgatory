// Fill out your copyright notice in the Description page of Project Settings.
//Test

#include "Portal.h"

// Sets default values
APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool APortal::IsActive()
{
	return bIsActive;
}

void APortal::SetActive(bool NewActive)
{
	bIsActive = NewActive;
}

void APortal::TeleportPlayer()
{
	//Temp teleport function, will be extended heavily later.
	player->SetActorLocation(target->GetActorLocation());
}

bool APortal::IsPlayerInFrontOfPortal(FVector Point, FVector PortalLocation, FVector PortalNormal)
{
	FPlane PortalPlane = FPlane(PortalLocation, PortalNormal);
	float PortalDot = PortalPlane.PlaneDot(Point);

	//If < 0 player is behind the plane.
	return (PortalDot >= 0);
}

bool APortal::IsPlayerCrossingPortal(FVector Point, FVector PortalLocation, FVector PortalNormal)
{
	FVector IntersectionPoint;
	//If returns >= 0, Player is in front
	FPlane PortalPlane = FPlane(PortalLocation, PortalNormal);

	float PortalDot = PortalPlane.PlaneDot(Point);
	bool IsCrossing = false;
	bool IsInFront = PortalDot >= 0;

	bool IsIntersecting = FMath::SegmentPlaneIntersection(LastPosition, Point, PortalPlane, IntersectionPoint);

	//Did we intersect the portal since last location
	//If yes, check direction, crossing forward means we were in front and now the back.

	if (IsIntersecting && !IsInFront && LastInFront)
	{
		IsCrossing = true;
	}

	LastInFront = IsInFront;
	LastPosition = Point;

	return IsCrossing;
}



