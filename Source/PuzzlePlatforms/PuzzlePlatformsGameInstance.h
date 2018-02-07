// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer);
	
	virtual void Init();

	/*
	Eligible Classes for implementing exec:
		- Player Controllers
		- Possessed Pawns
		- HUDs
		- Cheat Managers
		- Game Modes
		- Game Instances
	*/
	UFUNCTION(exec)
	void Host();

	UFUNCTION(exec)
	void Join(const FString& IpAddress);
	
};
