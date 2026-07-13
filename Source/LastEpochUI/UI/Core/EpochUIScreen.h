// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "EpochUIScreen.generated.h"

class UEpochUILayer;
class UEpochUIScreenRule;
struct FScreenInstanceData;

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
	
	UPROPERTY(EditDefaultsOnly, Category="Screen")
    FName ShowAnimationName;
    
    UPROPERTY(EditDefaultsOnly, Category="Screen")
    FName HideAnimationName;
	
	UFUNCTION(BlueprintPure)
	FGameplayTag GetScreenName() const { return ScreenName; }
	
	UFUNCTION(BlueprintCallable)
	TMap<FName, UWidgetAnimation*> GetAnimations() { return Animations; }
	
	void CacheAnimations();
	void AttachToLayer(UEpochUILayer* Layer) { ParentLayer = Layer; }
	bool IsAnimating() const { return GetWorld()->GetTimerManager().IsTimerActive(AnimTimerHandle); }
	void TriggerShow();
	void TriggerHide();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Screen")
	FGameplayTag ScreenName;
	
	UPROPERTY(BlueprintReadOnly)
	UEpochUILayer* ParentLayer;
	
	UPROPERTY(EditDefaultsOnly, Category="Screen")
	bool bUseBlueprintTransitionLogic;
	
	UPROPERTY()
	TMap<FName, UWidgetAnimation*> Animations;
	
	UPROPERTY(EditDefaultsOnly, Category="Screen")
	TArray<TSubclassOf<UEpochUIScreenRule>> OnOpenRules;
	
	UPROPERTY()
	TArray<TObjectPtr<UEpochUIScreenRule>> OnOpenRuleInstances;
	
	UPROPERTY()
	TArray<TObjectPtr<UEpochUIScreenRule>> OnCloseRuleInstances;
	
	UPROPERTY(EditDefaultsOnly, Category="Screen")
	TArray<TSubclassOf<UEpochUIScreenRule>> OnCloseRules;
	
	FTimerHandle AnimTimerHandle;
	
	UFUNCTION(BlueprintNativeEvent)
    void Show();
    
    UFUNCTION(BlueprintCallable)
    void BroadcastOnShowFinished();
    
    UFUNCTION(BlueprintNativeEvent)
    void Hide();
    
    UFUNCTION(BlueprintCallable)
    void BroadcastOnHideFinished();
	
	virtual void NativeConstruct() override;
};
