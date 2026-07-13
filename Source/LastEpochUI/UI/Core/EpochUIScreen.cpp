// Fill out your copyright notice in the Description page of Project Settings.


#include "EpochUIScreen.h"

#include "EpochUIManagementSubsystem.h"
#include "EpochUIScreenRule.h"
#include "MovieScene.h"
#include "Animation/WidgetAnimation.h"
#include "LastEpochUI/LastEpochUI.h"

void UEpochUIScreen::NativeConstruct()
{
	Super::NativeConstruct();
	CacheAnimations();
	
	UEpochUIManagementSubsystem* UIManager = GetGameInstance()->GetSubsystem<UEpochUIManagementSubsystem>();
	
	if (!UIManager)
	{
		UE_LOG(LogGame, Error, TEXT("Failed to find UI Management SubSystem for screen %s. Screen rules will not be recognized."), *ScreenName.ToString());
		return;
	}
	
	for (const TSubclassOf<UEpochUIScreenRule>& RuleClass : OnOpenRules)
	{
		UEpochUIScreenRule* NewInstance = NewObject<UEpochUIScreenRule>(this, RuleClass);
		NewInstance->PassUIManager(UIManager);
		NewInstance->AttachToScreen(this);
		OnOpenRuleInstances.Add(NewInstance);
	}
	
	for (const TSubclassOf<UEpochUIScreenRule>& RuleClass : OnCloseRules)
	{
		UEpochUIScreenRule* NewInstance = NewObject<UEpochUIScreenRule>(this, RuleClass);
		NewInstance->PassUIManager(UIManager);
		NewInstance->AttachToScreen(this);
		OnCloseRuleInstances.Add(NewInstance);
	}
}

void UEpochUIScreen::CacheAnimations()
{
	Animations.Empty();

	for (TFieldIterator<FObjectProperty> It(GetClass()); It; ++It)
	{
		if (const FObjectProperty* Prop = *It; Prop->PropertyClass == UWidgetAnimation::StaticClass())
		{
			if (UWidgetAnimation* Anim = Cast<UWidgetAnimation>(Prop->GetObjectPropertyValue_InContainer(this)))
			{
				if (Anim->MovieScene)
				{
					Animations.Add(Anim->MovieScene->GetFName(), Anim);
				}
			}
		}
	}
}

void UEpochUIScreen::TriggerShow()
{
	for (const TObjectPtr<UEpochUIScreenRule>& OnOpenRule : OnOpenRuleInstances)
	{
		if (OnOpenRule)
			OnOpenRule->Execute();
	}
	
	if (bUseBlueprintTransitionLogic)
	{
		Show();
		return;
	}
	
	SetVisibility(ESlateVisibility::Visible);
	UWidgetAnimation* ShowAnimation = Animations.FindRef(ShowAnimationName);
	if (!ShowAnimation)
	{
		if (!ShowAnimationName.IsNone())
		{
			UE_LOG(LogGame, Warning, TEXT("Failed to find widget animation %s for screen %s. Check to ensure a widget animation exists with that name."), *ShowAnimationName.ToString(), *ScreenName.ToString());
		}
		
		BroadcastOnShowFinished();
		return;
	}
	
	PlayAnimation(ShowAnimation);
	GetWorld()->GetTimerManager().SetTimer(AnimTimerHandle, this, &UEpochUIScreen::BroadcastOnShowFinished, ShowAnimation->GetEndTime());
}

void UEpochUIScreen::Show_Implementation()
{
	BroadcastOnShowFinished();
}

void UEpochUIScreen::TriggerHide()
{
	for (const TObjectPtr<UEpochUIScreenRule>& OnCloseRule : OnCloseRuleInstances)
	{
		if (OnCloseRule)
			OnCloseRule->Execute();
	}
	
	if (bUseBlueprintTransitionLogic)
	{
		Hide();
		return;
	}
	
	UWidgetAnimation* HideAnimation = Animations.FindRef(HideAnimationName);
	if (!HideAnimation)
	{
		if (!HideAnimationName.IsNone())
		{
			UE_LOG(LogGame, Warning, TEXT("Failed to find widget animation %s for screen %s. Check to ensure a widget animation exists with that name."), *HideAnimationName.ToString(), *ScreenName.ToString());
		}
		
		BroadcastOnHideFinished();
		return;
	}
	
	PlayAnimation(HideAnimation);
	GetWorld()->GetTimerManager().SetTimer(AnimTimerHandle, this, &UEpochUIScreen::BroadcastOnHideFinished, HideAnimation->GetEndTime());
}

void UEpochUIScreen::Hide_Implementation()
{
	BroadcastOnHideFinished();
}

void UEpochUIScreen::BroadcastOnShowFinished()
{
	OnShowFinished.Broadcast();
}

void UEpochUIScreen::BroadcastOnHideFinished()
{
	SetVisibility(ESlateVisibility::Collapsed);
	OnHideFinished.Broadcast();
}


