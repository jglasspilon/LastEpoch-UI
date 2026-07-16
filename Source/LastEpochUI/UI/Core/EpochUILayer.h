// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/Overlay.h"
#include "EpochUILayer.generated.h"

struct FGameplayTag;
class UEpochUIScreen;

UCLASS()
class LASTEPOCHUI_API UEpochUILayer : public UOverlay
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetLayerName() const { return LayerName; }
	
	void AddScreen(UEpochUIScreen* ScreenToAdd);
	
	UFUNCTION(BlueprintCallable, Category="Epoch UI")
	void ShowScreen(const FGameplayTag ScreenName);
	
	UFUNCTION(BlueprintCallable, Category="Epoch UI")
	void ToggleScreen(const FGameplayTag ScreenName);
	
	UFUNCTION(BlueprintCallable, Category="Epoch UI")
	void HideScreen();
	
	UFUNCTION(BlueprintCallable, Category="Epoch UI")
	bool IsScreenActive(const FGameplayTag ScreenName);
	
protected:
	UPROPERTY(EditAnywhere, Category="Layer")
	FGameplayTag LayerName;
	
	UPROPERTY(VisibleAnywhere)
	TMap<FGameplayTag, UEpochUIScreen*> LoadedScreens;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UEpochUIScreen> ActiveScreen;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UEpochUIScreen> PendingScreen;
	
	UFUNCTION()
	void OnActiveScreenShown();
	
	UFUNCTION()
	void OnActiveScreenHidden();
	
	virtual void OnSlotAdded(UPanelSlot* InSlot) override;
	void ChangeScreen(const FGameplayTag ScreenName);
};
