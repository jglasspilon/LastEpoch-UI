// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "EpochUIScreen.generated.h"

class UEpochUIScreenRule;
struct FScreenInstanceData;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FScreenTransitionEvent);

UCLASS(Abstract, Blueprintable, BlueprintType)
class LASTEPOCHUI_API UEpochUIScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FScreenTransitionEvent OnShowFinished;
	
	UPROPERTY(BlueprintAssignable)
	FScreenTransitionEvent OnHideFinished;
	
	UFUNCTION(BlueprintPure)
	FGameplayTag GetScreenName() const { return ScreenName; }
	
	void TriggerShow();
	void TriggerHide();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Screen")
	FGameplayTag ScreenName;
	
	UPROPERTY(EditDefaultsOnly, Category="Screen")
	TArray<TSubclassOf<UEpochUIScreenRule>> OnOpenRules;
	
	UPROPERTY()
	TArray<TObjectPtr<UEpochUIScreenRule>> OnOpenRuleInstances;
	
	UPROPERTY()
	TArray<TObjectPtr<UEpochUIScreenRule>> OnCloseRuleInstances;
	
	UPROPERTY(EditDefaultsOnly, Category="Screen")
	TArray<TSubclassOf<UEpochUIScreenRule>> OnCloseRules;
	
	UFUNCTION(BlueprintNativeEvent)
    void Show();
    
    UFUNCTION(BlueprintCallable)
    void BroadcastOnShowFinished() const;
    
    UFUNCTION(BlueprintNativeEvent)
    void Hide();
    
    UFUNCTION(BlueprintCallable)
    void BroadcastOnHideFinished() const;
	
	virtual void NativeConstruct() override;
};
