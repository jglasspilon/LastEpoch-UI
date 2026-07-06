// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "EpochUILayout.generated.h"

struct FScreenInstanceData;
class UEpochUILayer;

UCLASS(Blueprintable)
class LASTEPOCHUI_API UEpochUILayout : public UUserWidget
{
	GENERATED_BODY()
	
protected: 
	UPROPERTY(EditDefaultsOnly, Category="Screens")
	TArray<FScreenInstanceData> ScreensToLoad;
	
	UPROPERTY(EditDefaultsOnly, Category="Layers")
	TArray<FGameplayTag> LayersThatCloseAllOtherLayers;
	
	UPROPERTY(VisibleAnywhere, Category="Layers")
	TMap<FGameplayTag, UEpochUILayer*> Layers;
	
	UPROPERTY(VisibleAnywhere, Category="Layers")
	TMap<FGameplayTag, UEpochUILayer*> MappedLayersToScreenName;
	
	UFUNCTION(BlueprintCallable)
	void ShowScreen(FGameplayTag ScreenName);
	
	virtual void NativeConstruct() override;
	void GatherLayers();
	void LoadScreens(const TArray<FScreenInstanceData>& ToLoad);
};
