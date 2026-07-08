// Fill out your copyright notice in the Description page of Project Settings.


#include "EpochUILayer.h"
#include "EpochUIScreen.h"
#include "GameplayTagContainer.h"
#include "Components/OverlaySlot.h"
#include "LastEpochUI/LastEpochUI.h"

void UEpochUILayer::AddScreen(UEpochUIScreen* ScreenToAdd)
{
	if (!ScreenToAdd)
	{
		UE_LOG(LogGame, Warning, TEXT("Attempted to add null pointer to layer %s"), *LayerName.ToString())
		return;
	}
	
	if (LoadedScreens.Contains(ScreenToAdd->GetScreenName()))
	{
		UE_LOG(LogGame, Warning, TEXT("Attempted to add duplicate screen %s to layer %s"), *ScreenToAdd->GetScreenName().ToString(), *LayerName.ToString())
		return;
	}
	
	AddChild(ScreenToAdd);
	LoadedScreens.Add(ScreenToAdd->GetScreenName(), ScreenToAdd);
	ScreenToAdd->SetVisibility(ESlateVisibility::Collapsed);
	UE_LOG(LogGame, Log, TEXT("Added screen %s to layer %s"), *ScreenToAdd->GetScreenName().ToString(), *LayerName.ToString())
}

void UEpochUILayer::ChangeScreen(const FGameplayTag ScreenName)
{
	UEpochUIScreen* NextScreen = LoadedScreens.FindRef(ScreenName);
	
	if (!LoadedScreens.Contains(ScreenName))
	{
		UE_LOG(LogGame, Warning, TEXT("Failed to change to screen. %s screen has not been loaded on layer %s"), *ScreenName.ToString(), *LayerName.ToString())
		return;
	}
	
	if (ActiveScreen)
	{
		if (ActiveScreen->IsAnimating())
		{
			UE_LOG(LogGame, Warning, TEXT("Can't change screen, currently active screen is already changing."));
			return;
		}
		
		UE_LOG(LogGame, Log, TEXT("Changing screen on layer %s. From %s to %s"), *LayerName.ToString(), *ActiveScreen->GetScreenName().ToString(), *NextScreen->GetScreenName().ToString());
		
		PendingScreen = NextScreen;
		
		// Wait for hide to finish
		ActiveScreen->OnHideFinished.AddUniqueDynamic(this, &UEpochUILayer::OnActiveScreenHidden);
		ActiveScreen->TriggerHide();
	}
	else
	{
		UE_LOG(LogGame, Log, TEXT("Changing screen on layer %s. From empty to %s"), *LayerName.ToString(), *NextScreen->GetScreenName().ToString());
		ActiveScreen = NextScreen;
		
		// Wait for show to finish
		ActiveScreen->OnShowFinished.AddUniqueDynamic(this, &UEpochUILayer::OnActiveScreenShown);
		ActiveScreen->TriggerShow();
	}
}

void UEpochUILayer::ShowScreen(const FGameplayTag ScreenName)
{
	if (ActiveScreen && ActiveScreen->GetScreenName() == ScreenName)
	{
		return;
	}
	
	ChangeScreen(ScreenName);
}

void UEpochUILayer::ToggleScreen(const FGameplayTag ScreenName)
{
	if (ActiveScreen && ActiveScreen->GetScreenName() == ScreenName)
	{
		HideScreen();
		return;
	}
	
	ChangeScreen(ScreenName);
}

void UEpochUILayer::HideScreen()
{
	if (!ActiveScreen)
	{
		return;
	}
	
	if (ActiveScreen->IsAnimating())
	{
		UE_LOG(LogGame, Warning, TEXT("Can't hide screen, currently active screen is already changing."));
		return;
	}
	
	UE_LOG(LogGame, Log, TEXT("Changing screen on layer %s. From %s to empty"), *LayerName.ToString(), *ActiveScreen->GetScreenName().ToString());
     		
    // Wait for hide to finish
	ActiveScreen->OnHideFinished.AddUniqueDynamic(this, &UEpochUILayer::OnActiveScreenHidden);
    ActiveScreen->TriggerHide();
}

void UEpochUILayer::OnActiveScreenHidden()
{
	if (ActiveScreen)
	{
		ActiveScreen->OnHideFinished.RemoveDynamic(this, &UEpochUILayer::OnActiveScreenHidden);
	}
	
	if (PendingScreen)
	{
		ActiveScreen = PendingScreen;
		PendingScreen = nullptr;
		
		// Wait for show to finish
        ActiveScreen->OnShowFinished.AddUniqueDynamic(this, &UEpochUILayer::OnActiveScreenShown);
		ActiveScreen->TriggerShow();
	}
	else
	{
		ActiveScreen = nullptr;
	}
}

void UEpochUILayer::OnActiveScreenShown()
{
	if (ActiveScreen)
	{
		ActiveScreen->OnShowFinished.RemoveDynamic(this, &UEpochUILayer::OnActiveScreenShown);
	}
	
	// vv-Ability to add logic on screen shown finished in the future-vv //
}

void UEpochUILayer::OnSlotAdded(UPanelSlot* InSlot)
{
	Super::OnSlotAdded(InSlot);
	
	// Force the newly added slot entry to scale to the full width and height of the layer 
	if (UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(InSlot))
	{
		OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
        OverlaySlot->SetVerticalAlignment(VAlign_Fill);
	}
}


