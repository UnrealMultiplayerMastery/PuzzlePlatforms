// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("My Game Session");


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	/*
	// Called in Editor and at begin play
	UE_LOG(LogTemp, Warning, TEXT("Game instance constructor"))
	*/

	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MenuClass = MenuBPClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;
	InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	/*
	// Called at begin play only
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"))
	*/
	if (!ensure(MenuClass != nullptr)) return;
	if (!ensure(InGameMenuClass != nullptr)) return;
	///UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *MenuClass->GetName())
	///UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *InGameMenuClass->GetName())

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString())
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);

			// New: creates something on the heap instead of stack
			// MakeShareable: takes ordinary cpp pointer and makes it a shared pointer
			// ToSharedRef: convert shared pointer (can be null) to shared ref (cannot be null) 
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = true; // local network search only
			// SessionSearch->QuerySettings.Set(); // when using steam search

			if (SessionSearch.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Finding sessions"))
				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"))
	}
}

void UPuzzlePlatformsGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;
	
	Menu->Setup();

	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;
	UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();

	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;        // search over local network only
		SessionSettings.NumPublicConnections = 2;  // set num players
		SessionSettings.bShouldAdvertise = true;   // avoid having to send out an invite

		/*Session gets created asynchronously and a delegate gets sent back: OnCreateSessionCompleteDelegates*/
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating new session"))
		CreateSession();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to destroy session"))
		return;
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success && SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Finding sessions complete"))
		for (FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Session found: %s"), *SearchResult.GetSessionIdStr())
		}
	}
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to create session"))
		return;
	}

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

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}
