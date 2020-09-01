#include "FollowingWall.h"
#include "Kismet/GameplayStatics.h"

AFollowingWall::AFollowingWall()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFollowingWall::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}
	if (PlayerCharacter != nullptr)
	{
		PlayerFacingStart = PlayerCharacter->GetActorForwardVector();
	}
	UpVector = GetActorUpVector();
}

bool AFollowingWall::CheckDistance()
{
	FVector ActorCurrentLocation = GetActorLocation();
	FVector PlayerCurrentLocation = PlayerCharacter->GetActorLocation();
	if (FVector::Distance(PlayerCurrentLocation, ActorCurrentLocation) < DistanceToPlayer)
	{
		return false;
	}
	if (FVector::Distance(PlayerCurrentLocation, DistanceFirst->GetComponentLocation()) < (DistanceToPlayer * 6.0f))
	{
		return false;
	}
	if (FVector::Distance(PlayerCurrentLocation, DistanceSecond->GetComponentLocation()) < (DistanceToPlayer * 3.0f))
	{
		return false;
	}
	if (FVector::Distance(PlayerCurrentLocation, DistanceThird->GetComponentLocation()) < (DistanceToPlayer * 3.0f))
	{
		return false;
	}
	if (FVector::Distance(PlayerCurrentLocation, DistanceFourth->GetComponentLocation()) < (DistanceToPlayer * 6.0f))
	{
		return false;
	}
	return true;
}

FVector AFollowingWall::CalculateLocation()
{
	NewLocation = PlayerFacingStart;
	NewLocation.X *= DistanceToPlayer;
	NewLocation.Y *= DistanceToPlayer;
	if (NewLocation.X < 0.0f || NewLocation.Y < 0.0f)
	{
		NewLocation = PlayerCharacter->GetActorLocation() - NewLocation;
	}
	else
	{
		NewLocation = PlayerCharacter->GetActorLocation() + NewLocation;
	}
	return NewLocation;
}

void AFollowingWall::MoveActor(FVector Destination)
{
	SetActorLocation(Destination);
}

void AFollowingWall::RotateActorYaw(float Degrees)
{
	PlayerFacingStart = PlayerFacingStart.RotateAngleAxis(Degrees, UpVector);
	FRotator NewRotation = GetActorRotation().Add(0.0f, Degrees, 0.0f);
	SetActorRotation(NewRotation);
}

// Called every frame
void AFollowingWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CheckDistance())
	{
		MoveActor(CalculateLocation());
	}
}