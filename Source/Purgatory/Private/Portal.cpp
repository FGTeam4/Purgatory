// Fill out your copyright notice in the Description page of Project Settings.
//Test

#include "Portal.h"
#include "PortalManager.h"

// Sets default values
APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsActive = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->Mobility = EComponentMobility::Static;

	PortalRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PortalRootComponent"));
	PortalRootComponent->SetupAttachment(GetRootComponent());
	PortalRootComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	PortalRootComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	PortalRootComponent->Mobility = EComponentMobility::Static;

}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();

	if (Player == nullptr)
	{
		Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}

	if (TargetObject != nullptr)
	{
		UBoxComponent* TargetCollider = nullptr;
		TargetCollider = TargetObject->FindComponentByClass<UBoxComponent>();
		TargetObject->SetActorEnableCollision(false);
	}
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AActor* APortal::GetTarget()
{
	return TargetObject;
}

void APortal::SetTarget(AActor* NewTarget)
{
	TargetObject = NewTarget;
}

bool APortal::IsActive()
{
	return bIsActive;
}

void APortal::SetActive(bool NewActive)
{
	bIsActive = NewActive;
}

void APortal::TeleportPlayer(AActor* ActorToTeleport)
{
	if (ActorToTeleport != nullptr && bIsActive)
	{
		if (Player == nullptr)
		{
			Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		}

		FVector Velocity = FVector::ZeroVector;
		APurgatoryCharacter* PlayerChar = nullptr;

		if (ActorToTeleport->IsA(APurgatoryCharacter::StaticClass()) && ActorToTeleport != nullptr)
		{
			PlayerChar = Cast<APurgatoryCharacter>(ActorToTeleport);
			Velocity = PlayerChar->GetVelocity();
		}

		//Calculate and apply new location
		FHitResult HitResult;
		FVector NewLocation = ConvertLocationToActorSpace(ActorToTeleport->GetActorLocation(), this, TargetObject);
		ActorToTeleport->SetActorLocation(NewLocation, false, &HitResult, ETeleportType::TeleportPhysics);

		FRotator NewRotation = ConvertRotationToActorSpace(ActorToTeleport->GetActorRotation(), this, TargetObject);
		ActorToTeleport->SetActorRelativeRotation(NewRotation);

		FVector Dots;
		Dots.X = FVector::DotProduct(Velocity, GetActorForwardVector());
		Dots.Y = FVector::DotProduct(Velocity, GetActorRightVector());
		Dots.Z = FVector::DotProduct(Velocity, GetActorUpVector());

		FVector NewVelocity = Dots.X * TargetObject->GetActorForwardVector() +
			Dots.Y * TargetObject->GetActorRightVector() +
			Dots.Z * TargetObject->GetActorUpVector();

		//Player->GetCharacterMovement()->Velocity = NewVelocity;

		LastPosition = NewLocation;
	}
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

	//if (IsIntersecting && !IsInFront && bLastInFront)
	//{
	//	IsCrossing = true;
	//}

	if (IsIntersecting && !IsInFront)
	{
		IsCrossing = true;
	}

	bLastInFront = IsInFront;
	LastPosition = Point;

	return IsCrossing;
}

FVector APortal::ConvertLocationToActorSpace(FVector Location, AActor* Ref, AActor* Target)
{
	if (Ref == nullptr || Target == nullptr)
	{
		return FVector::ZeroVector;
	}

	FVector Direction = Location - Ref->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();

	FVector Dots;
	Dots.X = FVector::DotProduct(Direction, Ref->GetActorForwardVector());
	Dots.Y = FVector::DotProduct(Direction, Ref->GetActorRightVector());
	Dots.Z = FVector::DotProduct(Direction, Ref->GetActorUpVector());

	FVector NewDirection = Dots.X * Target->GetActorForwardVector()
		+ Dots.Y * Target->GetActorRightVector()
		+ Dots.Z * Target->GetActorUpVector();

	return TargetLocation + NewDirection;
}

FRotator APortal::ConvertRotationToActorSpace(FRotator Rotation, AActor* Ref, AActor* Target)
{
	if (Ref == nullptr || Target == nullptr)
	{
		return FRotator::ZeroRotator;
	}

	FTransform SourceTransform = Ref->GetActorTransform();
	FTransform TargetTransform = Target->GetActorTransform();

	FQuat QuatRotation = FQuat(Rotation);

	FQuat LocalQuat = SourceTransform.GetRotation().Inverse() * QuatRotation;
	FQuat NewWorldQuat = TargetTransform.GetRotation() * LocalQuat;

	return NewWorldQuat.Rotator();
}

bool APortal::IsPointInsideBox(FVector Point, UBoxComponent* Box)
{
	if (Box != nullptr)
	{
		FVector Center = Box->GetComponentLocation();
		FVector Half = Box->GetScaledBoxExtent();
		FVector DirectionX = Box->GetForwardVector();
		FVector DirectionY = Box->GetRightVector();
		FVector DirectionZ = Box->GetUpVector();

		FVector Direction = Point - Center;

		bool IsInside(FMath::Abs(FVector::DotProduct(Direction, DirectionX)) <= Half.X &&
			FMath::Abs(FVector::DotProduct(Direction, DirectionY)) <= Half.Y &&
			FMath::Abs(FVector::DotProduct(Direction, DirectionZ)) <= Half.Z);

		return IsInside;
	}

	return false;
}

AActor* APortal::GetPortalManager(AActor* Context)
{
	if (Context != nullptr)
	{
		APortalManager* Manager = nullptr;

		if (Context != nullptr && Context->GetWorld() != nullptr)
		{
			for (TObjectIterator<APortalManager> Itr; Itr; ++Itr)
			{
				Manager = *Itr;
			}
		}

		return Manager;
	}

	return nullptr;
}
