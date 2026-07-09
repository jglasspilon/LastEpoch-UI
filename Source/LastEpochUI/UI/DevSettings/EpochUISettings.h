// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "EpochUISettings.generated.h"

class UInputMappingContext;
class UInputAction;
class UEpochUILayout;

UCLASS(Config="Game UI", DefaultConfig)
class LASTEPOCHUI_API UEpochUISettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditDefaultsOnly, Category="UI Layout")
	TSoftClassPtr<UEpochUILayout> UILayoutWidget;
	
	UPROPERTY(Config, EditDefaultsOnly, Category="UI Input Binding")
	TSoftObjectPtr<UInputMappingContext> UIInputMappingContext;
	
	UPROPERTY(Config, EditDefaultsOnly, Category="UI Input Binding")
	TMap<TSoftObjectPtr<UInputAction>, FGameplayTag> ScreenInputActionMap;
	
	virtual FName GetCategoryName() const override
	{
		return FApp::GetProjectName();
	}
};
