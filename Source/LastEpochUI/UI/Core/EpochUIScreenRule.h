// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EpochUIScreenRule.generated.h"

class UEpochUIScreen;
class UEpochUIManagementSubsystem;
class UEpochUILayout;

UCLASS(Blueprintable, Abstract)
class LASTEPOCHUI_API UEpochUIScreenRule : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void Execute();
	
	void PassUIManager(UEpochUIManagementSubsystem* NewUIManager)
	{
		UIManager = NewUIManager;
	}
	
	void AttachToScreen(UEpochUIScreen* UIScreen)
	{
		ParentScreen = UIScreen;
	}
	
protected:
	
	UPROPERTY(BlueprintReadOnly)
	UEpochUIManagementSubsystem* UIManager;
	
	UPROPERTY(BlueprintReadOnly)
	UEpochUIScreen* ParentScreen;
};
