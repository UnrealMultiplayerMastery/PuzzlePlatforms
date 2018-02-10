// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "ServerRow.h"


UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostButtonMain != nullptr)) return false;
	HostButtonMain->OnReleased.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButtonMain != nullptr)) return false;
	JoinButtonMain->OnReleased.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(QuitButtonMain != nullptr)) return false;
	QuitButtonMain->OnReleased.AddDynamic(this, &UMainMenu::QuitPressed);

	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnReleased.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnReleased.AddDynamic(this, &UMainMenu::JoinServer);

	return true;
}

void UMainMenu::HostServer()
{
	if (!ensure(MenuInterface != nullptr)) return;
	MenuInterface->Host();
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{	
	if (!ensure(ServerRowClass != nullptr)) return;

	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	ServerList->ClearChildren();

	uint32 i = 0;

	// FString&: avoid copying
	for (const FString& ServerName : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) continue;

		Row->ServerName->SetText(FText::FromString(ServerName));
		Row->Setup(this, i);
		++i; // pre-increment only slightly more efficient

		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectServerListIndex(uint32 Index)
{
	SelectedServerListIndex = Index;
}

void UMainMenu::JoinServer()
{
	// IpAddress manual input
	// if (!ensure(IpAddressField != nullptr) && MenuInterface != nullptr) return;

	if (SelectedServerListIndex.IsSet() && MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected server index: %d"), SelectedServerListIndex.GetValue())
		MenuInterface->Join(SelectedServerListIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Server index not set"))
	}

	/*
	// IpAddress manual input
	const FString& IpAddress = IpAddressField->GetText().ToString();
	MenuInterface->Join(IpAddress);
	*/
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitPressed()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}
