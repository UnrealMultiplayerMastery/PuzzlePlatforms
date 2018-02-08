// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


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

void UMainMenu::JoinServer()
{
	if (!ensure(IpAddressField != nullptr)) return;
	if (MenuInterface != nullptr)
	{
		const FString& IpAddress = IpAddressField->GetText().ToString();
		MenuInterface->Join(IpAddress);
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
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
