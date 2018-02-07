// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	/*
	// Called in Editor and at begin play
	UE_LOG(LogTemp, Warning, TEXT("Game instance constructor"))
	*/

	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MenuClass = MenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	/*
	// Called at begin play only
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"))
	*/
	if (!ensure(MenuClass != nullptr)) return;
	UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *MenuClass->GetName())
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(MenuClass != nullptr)) return;
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;
	
	Menu->Setup();

	Menu->SetMenuInterface(this);
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
	GEngine->AddOnScreenDebugMessage(
		-1, 5.f, FColor::Green,
		FString::Printf(TEXT("Joining: %s"), *IpAddress)
	);

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(IpAddress, ETravelType::TRAVEL_Absolute);
}
