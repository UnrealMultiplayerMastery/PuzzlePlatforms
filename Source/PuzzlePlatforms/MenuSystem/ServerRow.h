// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostUsername;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NumConnections;

	// State resides in MainMenu SelectedIndex
	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;
	
	void Setup(class UMainMenu* InParent, uint32 InIndex);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ServerRowButton;

	UPROPERTY()
	class UMainMenu* Parent;
	uint32 Index;

	UFUNCTION()
	void OnClicked();
};
