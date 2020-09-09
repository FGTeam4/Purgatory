#include "SoundManager.h"

// Sets default values
ASoundManager::ASoundManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASoundManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoundManager::RegisterPlayingSound(AActor* PlayingActor, USoundClass* PlayingSoundClass)
{
	PlayingActors.Add(PlayingActor, PlayingSoundClass);
}

void ASoundManager::SetSoundVolume(USoundClass* TargetSoundClass, float NewVolume)
{
	if (TargetSoundClass != nullptr)
	{
		TargetSoundClass->Properties.Volume = NewVolume;
	}
}

void ASoundManager::MuteSound(USoundClass* TargetSoundClass)
{
	if (TargetSoundClass != nullptr)
	{
		TargetSoundClass->Properties.Volume = 0.f;
	}
}

void ASoundManager::MuteAllSounds()
{
	for (const TPair<AActor*, USoundClass*>& Pair : PlayingActors)
	{
		Pair.Value->Properties.Volume = 0;
	}
}

bool ASoundManager::IsSoundPlaying(AActor* Actor, USoundClass* TargetSoundClass)
{
	if (Actor->GetComponentByClass(USoundClass::StaticClass()))
	{
		for (const TPair<AActor*, USoundClass*>& Pair : PlayingActors)
		{

		}
	}

	return false;
}
