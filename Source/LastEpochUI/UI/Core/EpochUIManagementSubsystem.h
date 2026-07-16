// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EpochUIManagementSubsystem.generated.h"

class UEpochUISettings;
struct FInputActionInstance;
class UInputAction;
class UEpochUILayout;

UCLASS(BlueprintType, Blueprintable)
class LASTEPOCHUI_API UEpochUIManagementSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Epoch UI")
	void ShowScreen(FGameplayTag ScreenName) const;
	
	UFUNCTION(BlueprintCallable, Category="Epoch UI")
	void HideScreen(FGameplayTag ScreenName) const;
	
	UFUNCTION(BlueprintCallable, Category="Epoch UI")
	void HideActiveScreenOfLayer(FGameplayTag LayerName) const;
	
	UFUNCTION(BlueprintCallable, Category="Epoch UI")
	bool IsScreenActive(FGameplayTag ScreenName) const;
	
	UFUNCTION(BlueprintCallable, Category="Epoch UI")
	TArray<FGameplayTag> GetLayerNames() const;
	
	UFUNCTION(BlueprintCallable, Category="Epoch UI")
	void EnableMenuToggling() { CanToggleMenus = true; }
	
	UFUNCTION(BlueprintCallable, Category="Epoch UI")
	void DisableMenuToggling() { CanToggleMenus = false; }
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
protected:
	UPROPERTY()
	TObjectPtr<UEpochUILayout> UILayoutInstance;
	
	UPROPERTY()
	TMap<TObjectPtr<UInputAction>, FGameplayTag> ScreenInputActionMap;
	
	bool CanToggleMenus = true;
	
	void OnUILayoutLoaded(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject);
	void OnMappingContextLoaded(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject) const;
	void BindInputs(const APlayerController* PC);
	void HandleScreenInput(const FInputActionInstance& Instance);
};
