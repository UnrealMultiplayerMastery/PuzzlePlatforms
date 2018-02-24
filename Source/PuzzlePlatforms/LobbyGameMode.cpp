// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"

#include <Runtime/Engine/Classes/Engine/Engine.h>


void ALobbyGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumPlayers;

	if (NumPlayers >= 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("3 players have joined!"))
		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;
		bUseSeamlessTravel = true;
		World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
}

void ALobbyGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
	--NumPlayers;
}
