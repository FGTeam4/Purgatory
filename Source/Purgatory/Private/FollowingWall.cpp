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

bool AFollowingWall::CheckMove()
{
	FVector ActorCurrentLocation = GetActorLocation();
	FVector PlayerCurrentLocation = PlayerCharacter->GetActorLocation();
	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("DistanceActor: %f /400"), FVector::Distance(PlayerCurrentLocation, ActorCurrentLocation)));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("DistanceFirst: %f /2400"), FVector::Distance(PlayerCurrentLocation, DistanceFirst->GetComponentLocation())));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("DistanceSecond: %f /1200"), FVector::Distance(PlayerCurrentLocation, DistanceSecond->GetComponentLocation())));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("DistanceThird: %f /1200"), FVector::Distance(PlayerCurrentLocation, DistanceThird->GetComponentLocation())));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DistanceFourth: %f /2400"), FVector::Distance(PlayerCurrentLocation, DistanceFourth->GetComponentLocation())));*/
	if (FVector::DistSquared(PlayerCurrentLocation, ActorCurrentLocation) < (DistanceToPlayer * DistanceToPlayer))
	{
		return false;
	}
	if (FVector::DistSquared(PlayerCurrentLocation, DistanceFirst->GetComponentLocation()) < (DistanceToPlayer * DistanceToPlayer * 6.0f))
	{
		return false;
	}
	if (FVector::DistSquared(PlayerCurrentLocation, DistanceSecond->GetComponentLocation()) < (DistanceToPlayer * DistanceToPlayer * 3.0f))
	{
		return false;
	}
	if (FVector::DistSquared(PlayerCurrentLocation, DistanceThird->GetComponentLocation()) < (DistanceToPlayer * DistanceToPlayer * 3.0f))
	{
		return false;
	}
	if (FVector::DistSquared(PlayerCurrentLocation, DistanceFourth->GetComponentLocation()) < (DistanceToPlayer * DistanceToPlayer * 6.0f))
	{
		return false;
	}
	return true;
}

void AFollowingWall::SetLocation()
{
	FVector NewLocation = PlayerFacingStart;
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
	SetActorLocation(NewLocation);
}

void AFollowingWall::RotateSelfYaw(float Degrees)
{
	PlayerFacingStart = PlayerFacingStart.RotateAngleAxis(Degrees, UpVector);
	FRotator NewRotation = GetActorRotation().Add(0.0f, Degrees, 0.0f);
	SetActorRotation(NewRotation);
}

// Called every frame
void AFollowingWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!Stop)
	{
		if (CheckMove())
		{
			SetLocation();
		}
	}
}