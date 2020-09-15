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

	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	PlayerFacingCurrent = PlayerCharacter->GetActorForwardVector();
	StartPosition = GetActorLocation();
	CalculateLocation();
	StartMoveActorTimer();
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
	ActorLocation = GetActorLocation();
	CurrentPlayerLocation = PlayerCharacter->GetActorLocation();
	MoveVector.Z = 0.0f;
	MoveVector.X = ActorLocation.X;
	MoveVector.Y = ActorLocation.Y;
	MoveVector = (CurrentPlayerLocation - MoveVector) * PlayerFacingCurrent;
	MoveVector.Z = 0.0f;
	if (PlayerFacingCurrent.X < 0.0f || PlayerFacingCurrent.Y < 0.0f)
	{
		MoveVector *= -1;
	}
}

void AFollowingWall::MoveActor()
{
	if (++CurrentMoveStep < MaxMoveStep)
	{
		FVector VectorLocation = ActorLocation + MoveVector * MoveAmount * CurrentMoveStep;
		VectorLocation.Z = CurrentPlayerLocation.Z;
		if (SetActorLocation(VectorLocation, true))
		{
			CurrentMoveStep = MaxMoveStep;
		}
		if (CheckDistance())
		{
			StopMoveActorTimer();
			CalculateLocation();
			StartMoveActorTimer();
		}
	}
	if (CheckDistance())
	{
		StopMoveActorTimer();
		CalculateLocation();
		StartMoveActorTimer();
	}
}

void AFollowingWall::ResetWall(FVector PlayerSpawnLocation)
{
	StopMoveActorTimer();
	SetActorLocation(PlayerSpawnLocation - PlayerFacingCurrent * DistanceToPlayer * RestartOffsetMultiplier);
	StartMoveActorTimer();
}

void AFollowingWall::ResetWallOnRestart()
{
	StopMoveActorTimer();
	PlayerFacingCurrent = PlayerFacingStart;
	SetActorRotation(FRotator(0.0f, StartingRotation, 0.0f));
	SetActorLocation(FVector(PlayerCharacter->GetActorLocation().X, StartPosition.Y, StartPosition.Z) - PlayerFacingCurrent * DistanceToPlayer * RestartOffsetMultiplier);
	StartMoveActorTimer();
}

void AFollowingWall::SetYawRotation(float Degrees)
{
	StopMoveActorTimer();
	FRotator NewRotation = FRotator(0.0f, Degrees, 0.0f);
	if (Degrees == 0.0f)
	{
		PlayerFacingCurrent.Set(0.0f, 1.0f, 0.0f);
	}
	else if (Degrees == 90.0f || Degrees == -270.0f)
	{
		PlayerFacingCurrent.Set(-1.0f, 0.0f, 0.0f);
	}
	else if (Degrees == 180.0f || Degrees == -180.0f)
	{
		PlayerFacingCurrent.Set(0.0f, -1.0f, 0.0f);
	}
	else if (Degrees == 270.0f || Degrees == -90.0f)
	{
		PlayerFacingCurrent.Set(1.0f, 0.0f, 0.0f);
	}
	SetActorRotation(NewRotation);
	StartMoveActorTimer();
}