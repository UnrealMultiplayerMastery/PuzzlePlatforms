// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUZZLEPLATFORMS_API IMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Pure virtual function
	// This has an empty implementation, expects the child class to implement
	virtual void Host(FString ServerName) = 0;

	// IpAddress field manual input
	// virtual void Join(const FString& IpAddress) = 0;

	virtual void Join(uint32 Index) = 0;

	virtual void LoadMainMenu() = 0;

	virtual void RefreshServerList() = 0;
	
};
