// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "EpochUIScreen.generated.h"

struct FScreenInstanceData;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FScreenTransitionEvent);

UCLASS(Abstract, Blueprintable, BlueprintType)
class LASTEPOCHUI_API UEpochUIScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FScreenTransitionEvent OnShowFinished;
	
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FScreenTransitionEvent OnHideFinished;
	
	UFUNCTION(BlueprintPure)
	FGameplayTag GetScreenName() const { return ScreenName; }
	
	UFUNCTION(BlueprintCallable)
	void SetScreenName(const FGameplayTag NewScreenName) { ScreenName = NewScreenName; }
	
	UFUNCTION(BlueprintNativeEvent)
	void Show();
	
	UFUNCTION(BlueprintCallable)
	void BroadcastOnShowFinished();
	
	UFUNCTION(BlueprintNativeEvent)
	void Hide();
	
	UFUNCTION(BlueprintCallable)
	void BroadcastOnHideFinished();
	
	void InitializeScreen(const FScreenInstanceData& ScreenData);
	
protected:
	UPROPERTY(VisibleAnywhere)
	FGameplayTag ScreenName;
};
