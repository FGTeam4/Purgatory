// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundClass.h"
#include "SoundManager.generated.h"

UCLASS()
class PURGATORY_API ASoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoundManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE bool GetIsMuted() { return bIsMuted; }

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void SetSoundVolume(USoundClass* TargetSoundClass, float NewVolume);

	void RegisterPlayingSound(AActor* PlayingActor, USoundClass* PlayingSoundClass);

	/* Mutes the sound of one object.*/
	void MuteSound(USoundClass* TargetSoundClass);

	void UnMuteSound(USoundClass* TargetSoundClass);


	/* Mutes the sound of all registered objects in the game*/ 
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void MuteRegisteredSounds();

	bool IsSoundPlaying(AActor* Actor, USoundClass* TargetSoundClass);

public:

private:

	bool bIsMuted;

	//Actors currently playing sounds.
	TMap<AActor*, USoundClass*> PlayingActors;
};
