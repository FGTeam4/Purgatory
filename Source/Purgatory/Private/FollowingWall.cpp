#include "FollowingWall.h"
#include "Kismet/GameplayStatics.h"

AFollowingWall::AFollowingWall()
{
}

void AFollowingWall::StartCheckDistanceTimer()
{
	GetWorld()->GetTimerManager().SetTimer(CheckDistanceTimerHandle, this, &AFollowingWall::CheckDistance, CheckDistanceInterval, true);
}

void AFollowingWall::StopCheckDistanceTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(CheckDistanceTimerHandle);
}

void AFollowingWall::StartMoveActorTimer()
{
	GetWorld()->GetTimerManager().SetTimer(MoveActorTimerHandle, this, &AFollowingWall::MoveActor, MoveActorInterval, true);
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
	}
	StartCheckDistanceTimer();
}

void AFollowingWall::CheckDistance()
{
	FVector ActorCurrentLocation = GetActorLocation();
	FVector PlayerCurrentLocation = PlayerCharacter->GetActorLocation();
	if (FVector::Distance(PlayerCurrentLocation, ActorCurrentLocation) < DistanceToPlayer)
	{
		return;
	}
	if (FVector::Distance(PlayerCurrentLocation, DistanceFirst->GetComponentLocation()) < (DistanceToPlayer * 6.0f))
	{
		return;
	}
	if (FVector::Distance(PlayerCurrentLocation, DistanceSecond->GetComponentLocation()) < (DistanceToPlayer * 3.0f))
	{
		return;
	}
	if (FVector::Distance(PlayerCurrentLocation, DistanceThird->GetComponentLocation()) < (DistanceToPlayer * 3.0f))
	{
		return;
	}
	if (FVector::Distance(PlayerCurrentLocation, DistanceFourth->GetComponentLocation()) < (DistanceToPlayer * 6.0f))
	{
		return;
	}
	StopCheckDistanceTimer();
	CalculateLocation();
}

void AFollowingWall::CalculateLocation()
{
	FVector ActorLocation = GetActorLocation();
	MoveVector.Z = 0.0f;
	MoveVector.X = ActorLocation.X;
	MoveVector.Y = ActorLocation.Y;
	MoveVector = (PlayerCharacter->GetActorLocation() - MoveVector);
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
	if (++CurrentMoveStep <= MoveSpeed)
	{
		if (PlayerFacingStart.Y > 0.0f)
		{
			MoveVector = GetActorLocation() - MoveVector * MoveAmount * CurrentMoveStep * PlayerFacingStart;
		}
		else
		{
			MoveVector = GetActorLocation() + MoveVector * MoveAmount * CurrentMoveStep * PlayerFacingStart;
		}
		if (!SetActorLocation(MoveVector, true))
		{
			StopMoveActorTimer();
			StartCheckDistanceTimer();
		}
	}
	else
	{
		StopMoveActorTimer();
		StartCheckDistanceTimer();
	}
}

void AFollowingWall::OnLevelReset()
{
	SetActorLocation(PlayerCharacter->GetActorLocation() - PlayerFacingStart * DistanceToPlayer);
	StopMoveActorTimer();
	StartCheckDistanceTimer();
}

void AFollowingWall::SetYawRotation(float Degrees)
{
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
	SetActorLocation(PlayerCharacter->GetActorLocation() - PlayerFacingStart * DistanceToPlayer * 4.0f);
	StopMoveActorTimer();
	StartCheckDistanceTimer();
}