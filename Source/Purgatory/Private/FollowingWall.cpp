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
}

void AFollowingWall::SetLocation()
{
	FVector NewLocation = PlayerFacingStart;
	NewLocation.X *= Distance;
	NewLocation.Y *= Distance;
	NewLocation = PlayerCharacter->GetActorLocation() - NewLocation;
	SetActorLocation(NewLocation);
}

// Called every frame
void AFollowingWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (PlayerCharacter != nullptr)
		{
			PlayerFacingStart = PlayerCharacter->GetActorForwardVector();
		}
	}
	else
	{
		if ((FVector::DistSquared(PlayerCharacter->GetActorLocation(), GetActorLocation())) > (Distance * Distance))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Distance: %f"), FVector::Distance(PlayerCharacter->GetActorLocation(), GetActorLocation())));
			SetLocation();
		}
	}
}