// Fill out your copyright notice in the Description page of Project Settings.

#include "EpochUILayout.h"
#include "EpochUILayer.h"
#include "EpochUIScreen.h"
#include "Blueprint/WidgetTree.h"
#include "LastEpochUI/LastEpochUI.h"
#include "LastEpochUI/UI/Structs/FScreenInstanceData.h"

void UEpochUILayout::NativeConstruct()
{
	Super::NativeConstruct();
	GatherLayers();
	LoadScreens(ScreensToLoad);
}

void UEpochUILayout::GatherLayers()
{
	Layers.Empty();
	TArray<UWidget*> AllLayers;
	WidgetTree->GetAllWidgets(AllLayers);
	
	for (UWidget* Widget : AllLayers)
	{
		if (UEpochUILayer* Layer = Cast<UEpochUILayer>(Widget))
		{
			Layers.Add(Layer->GetLayerName(), Layer);
		}
	}
}

void UEpochUILayout::LoadScreens(const TArray<FScreenInstanceData>& ToLoad)
{
	int32 index = 0;
	UWorld* World = GetWorld();
	
	if (!World)
	{
		UE_LOG(LogGame, Error, TEXT("Failed to load screens in UI Layout. No World Exists on Load attempt."))
		return;
	}
	
	for (const FScreenInstanceData& ScreenData : ToLoad)
	{
		index ++;
		if (!ScreenData.ScreenTypeToInstantiate)
		{
			UE_LOG(LogGame, Error, TEXT("Failed to load screen %i in UI Layout. No screen type selected."), index)
			continue;
		}
		
		UEpochUILayer* Layer = Layers.FindRef(ScreenData.OwningLayer);
		if (!Layer)
		{
			UE_LOG(LogGame, Error, TEXT("Failed to load screen %i in UI Layout. Parent layer %s not found in UI Layout."), index, *ScreenData.OwningLayer.ToString());
			continue;
		}
		
		UEpochUIScreen* NewScreen = CreateWidget<UEpochUIScreen>(World, ScreenData.ScreenTypeToInstantiate);
		if (!NewScreen)
		{
			UE_LOG(LogGame, Error, TEXT("Failed to load screen %i in UI Layout. Loaded null class."), index)
			continue;
		}
		
		Layer->AddScreen(NewScreen);
		MappedLayersToScreenName.Add(NewScreen->GetScreenName(), Layer);
		
		UE_LOG(LogGame, Log, TEXT("Successfully loaded screen %s to layer %s."), *NewScreen->GetScreenName().ToString(), *Layer->GetLayerName().ToString())
	}
}

void UEpochUILayout::ShowScreen(FGameplayTag ScreenName)
{
	UEpochUILayer* Layer = MappedLayersToScreenName.FindRef(ScreenName);
	if (!Layer)
	{
		UE_LOG(LogGame, Error, TEXT("Failed to Show screen %s. Name not recognized."), *ScreenName.ToString());
		return;
	}
	
	if (LayersThatCloseAllOtherLayers.Contains(Layer->GetLayerName()))
	{
		for (const TPair<FGameplayTag, UEpochUILayer*>& LayerToClose : Layers)
		{
			if (LayerToClose.Value->GetLayerName() == Layer->GetLayerName())
				continue;
			
			LayerToClose.Value->HideActiveScreen();
		}
	}
	
	Layer->ShowScreen(ScreenName);
}