// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer);
	
	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	/*
	Eligible Classes for implementing Exec:
		- Player Controllers
		- Possessed Pawns
		- HUDs
		- Cheat Managers
		- Game Modes
		- Game Instances
	*/
	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void Join(const FString& IpAddress);

private:
	TSubclassOf<class UUserWidget> MenuClass;
	
	class UMainMenu* Menu;
};
