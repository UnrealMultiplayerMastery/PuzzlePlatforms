// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"

#include <Runtime/Engine/Classes/Engine/Engine.h>
#include <TimerManager.h>
#include "PuzzlePlatformsGameInstance.h"

const static int MIN_NUM_PUBLIC_CONNECTIONS = 2;


void ALobbyGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumPlayers;

	if (NumPlayers >= MIN_NUM_PUBLIC_CONNECTIONS)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d players have joined!"), MIN_NUM_PUBLIC_CONNECTIONS)
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 5);
	}
}

void ALobbyGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
	--NumPlayers;
}

void ALobbyGameMode::StartGame()
{
	auto GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;

	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}
