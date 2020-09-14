#include "FollowingWall.h"
#include "Kismet/GameplayStatics.h"

AFollowingWall::AFollowingWall()
{
}

void AFollowingWall::StartMoveActorTimer()
{
	GetWorld()->GetTimerManager().SetTimer(MoveActorTimerHandle, this, &AFollowingWall::MoveActor, MoveActorInterval * GetWorld()->GetDeltaSeconds(), true);
}

void AFollowingWall::StopMoveActorTimer()
{
	CurrentMoveStep = 0;
	GetWorld()->GetTimerManager().ClearTimer(MoveActorTimerHandle);
}

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
		PlayerFacingStart.X = FGenericPlatformMath::RoundToFloat(PlayerFacingStart.X);
		PlayerFacingStart.Y = FGenericPlatformMath::RoundToFloat(PlayerFacingStart.Y);
		PlayerFacingStart.Z = FGenericPlatformMath::RoundToFloat(PlayerFacingStart.Z);
		SetActorLocation(PlayerCharacter->GetActorLocation() - PlayerFacingStart * DistanceToPlayer);
		CalculateLocation();
	}
}

bool AFollowingWall::CheckDistance()
{
	FVector ActorCurrentLocation = GetActorLocation();
	FVector PlayerCurrentLocation = PlayerCharacter->GetActorLocation();
	FVector ClosestDistance = ActorCurrentLocation;
	if (FVector::Distance(PlayerCurrentLocation, DistanceSecond->GetComponentLocation()) < FVector::Distance(PlayerCurrentLocation, ClosestDistance))
	{
		ClosestDistance = DistanceSecond->GetComponentLocation();
	}
	if (FVector::Distance(PlayerCurrentLocation, DistanceThird->GetComponentLocation()) < FVector::Distance(PlayerCurrentLocation, ClosestDistance))
	{
		ClosestDistance = DistanceThird->GetComponentLocation();
	}
	if (FVector::Distance(PlayerCurrentLocation, DistanceFirst->GetComponentLocation()) < FVector::Distance(PlayerCurrentLocation, ClosestDistance))
	{
		ClosestDistance = DistanceFirst->GetComponentLocation();
	}
	if (FVector::Distance(PlayerCurrentLocation, DistanceFourth->GetComponentLocation()) < FVector::Distance(PlayerCurrentLocation, ClosestDistance))
	{
		ClosestDistance = DistanceFourth->GetComponentLocation();
	}
	if (FVector::Distance(PlayerCurrentLocation, ClosestDistance) <= DistanceToPlayer)
	{
		return false;
	}
	return true;
}

void AFollowingWall::CalculateLocation()
{
	StopMoveActorTimer();
	ActorLocation = GetActorLocation();
	CurrentPlayerLocation = PlayerCharacter->GetActorLocation();
	MoveVector.Z = 0.0f;
	MoveVector.X = ActorLocation.X;
	MoveVector.Y = ActorLocation.Y;
	MoveVector = (CurrentPlayerLocation - MoveVector) * PlayerFacingStart;
	MoveVector.Z = 0.0f;
	if (PlayerFacingStart.X < 0.0f || PlayerFacingStart.Y < 0.0f)
	{
		MoveVector *= -1;
	}
	StartMoveActorTimer();
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
	if (++CurrentMoveStep < MoveSpeed)
	{
		FVector VectorLocation = ActorLocation + MoveVector * MoveAmount * CurrentMoveStep;
		VectorLocation.Z = CurrentPlayerLocation.Z;
		if (SetActorLocation(VectorLocation, true))
		{
			CurrentMoveStep = MoveSpeed;
		}
		if (CheckDistance())
		{
			CalculateLocation();
		}
	}
	else if(CheckDistance())
	{
		CalculateLocation();
	}
}

void AFollowingWall::ResetWall(FVector PlayerSpawnLocation)
{
	StopMoveActorTimer();
	SetActorLocation(PlayerSpawnLocation - PlayerFacingStart * DistanceToPlayer);
	StartMoveActorTimer();
}

void AFollowingWall::SetYawRotation(float Degrees)
{
	StopMoveActorTimer();
	FRotator NewRotation = FRotator(0.0f, Degrees, 0.0f);
	if (Degrees == 0.0f)
	{
		PlayerFacingStart.Set(0.0f, 1.0f, 0.0f);
	}
	else if (Degrees == 90.0f || Degrees == -270.0f)
	{
		PlayerFacingStart.Set(-1.0f, 0.0f, 0.0f);
	}
	else if (Degrees == 180.0f || Degrees == -180.0f)
	{
		PlayerFacingStart.Set(0.0f, -1.0f, 0.0f);
	}
	else if (Degrees == 270.0f || Degrees == -90.0f)
	{
		PlayerFacingStart.Set(1.0f, 0.0f, 0.0f);
	}
	SetActorRotation(NewRotation);
	StartMoveActorTimer();
}