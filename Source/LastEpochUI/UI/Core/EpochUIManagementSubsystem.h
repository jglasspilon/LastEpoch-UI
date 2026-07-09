// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/EngineSubsystem.h"
#include "EpochUIManagementSubsystem.generated.h"


class UEpochUISettings;
struct FInputActionInstance;
class UInputAction;
class UEpochUILayout;

UCLASS()
class LASTEPOCHUI_API UEpochUIManagementSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
protected:
	UPROPERTY()
	TObjectPtr<UEpochUILayout> UILayoutInstance;
	
	UPROPERTY()
	TMap<TObjectPtr<UInputAction>, FGameplayTag> ScreenInputActionMap;
	
	void OnUILayoutLoaded(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject);
	void OnMappingContextLoaded(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject);
	void BindInputs(const APlayerController* PC);
	void HandleScreenInput(const FInputActionInstance& Instance);
};
