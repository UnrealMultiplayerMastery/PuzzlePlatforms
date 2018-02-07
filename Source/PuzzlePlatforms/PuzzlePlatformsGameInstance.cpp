// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	/*
	// Called in Editor and at begin play
	UE_LOG(LogTemp, Warning, TEXT("Game instance constructor"))
	*/
}


void UPuzzlePlatformsGameInstance::Init()
{
	/*
	// Called at begin play only
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"))
	*/
}

void UPuzzlePlatformsGameInstance::Host()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Hosting game"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& IpAddress)
{
	///FString IpAddress = FString(TEXT("192.168.1.123"));
	GEngine->AddOnScreenDebugMessage(
		-1, 5.f, FColor::Green,
		FString::Printf(TEXT("Joining: %s"), *IpAddress)
	);

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(IpAddress, ETravelType::TRAVEL_Absolute);
}
