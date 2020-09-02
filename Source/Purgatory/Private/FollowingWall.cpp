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
		PlayerStartPosition = PlayerCharacter->GetActorLocation();
		PlayerFacingStart.X = FGenericPlatformMath::RoundToFloat(PlayerFacingStart.X);
		PlayerFacingStart.Y = FGenericPlatformMath::RoundToFloat(PlayerFacingStart.Y);
		PlayerFacingStart.Z = FGenericPlatformMath::RoundToFloat(PlayerFacingStart.Z);
	}
}

// Called every frame
void AFollowingWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CheckDistance())
	{
		CalculateLocation();
	}
	if (bMoving)
	{
		MoveActor();
	}
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

void AFollowingWall::CalculateLocation()
{
	FVector ActorLocation = GetActorLocation();
	MoveVector.Z = 0.0f;
	MoveVector.X = ActorLocation.X;
	MoveVector.Y = ActorLocation.Y;
	MoveVector = (PlayerCharacter->GetActorLocation() - MoveVector);
	bMoving = true;
}

void AFollowingWall::MoveActor()
{
	if (PlayerFacingStart.X != 0.0f)
	{
		SetActorLocation(FVector(GetActorLocation().X, PlayerCharacter->GetActorLocation().Y, GetActorLocation().Z));
	}
	else if (PlayerFacingStart.Y != 0.0f)
	{
		SetActorLocation(FVector(PlayerCharacter->GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));
	}
	if (++CurrentMoveStep <= MoveSpeed)
	{
		if (PlayerFacingStart.X < 0.0f || PlayerFacingStart.Y < 0.0f)
		{
			MoveVector = GetActorLocation() - MoveVector * MoveAmount * CurrentMoveStep * PlayerFacingStart;
		}
		else
		{
			MoveVector = GetActorLocation() + MoveVector * MoveAmount * CurrentMoveStep * PlayerFacingStart;
		}
		if (!SetActorLocation(MoveVector, true))
		{
			CurrentMoveStep = 0;
			bMoving = false;
		}
	}
	else
	{
		CurrentMoveStep = 0;
		bMoving = false;
	}
}

void AFollowingWall::OnLevelReset()
{
	PlayerStartPosition = PlayerCharacter->GetActorLocation();
	SetActorLocation(PlayerStartPosition - PlayerFacingStart * DistanceToPlayer);
}

void AFollowingWall::RotateActorYaw(float Degrees)
{
	PlayerFacingStart = PlayerFacingStart.RotateAngleAxis(Degrees, GetActorUpVector());
	PlayerFacingStart.X = FGenericPlatformMath::RoundToFloat(PlayerFacingStart.X);
	PlayerFacingStart.Y = FGenericPlatformMath::RoundToFloat(PlayerFacingStart.Y);
	PlayerFacingStart.Z = FGenericPlatformMath::RoundToFloat(PlayerFacingStart.Z);
	FRotator NewRotation = GetActorRotation().Add(0.0f, Degrees, 0.0f);
	SetActorRotation(NewRotation);
	SetActorLocation(PlayerCharacter->GetActorLocation() - PlayerFacingStart * DistanceToPlayer * 4.0f);
}