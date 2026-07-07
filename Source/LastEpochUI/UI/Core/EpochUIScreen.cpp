// Fill out your copyright notice in the Description page of Project Settings.


#include "EpochUIScreen.h"
#include "EpochUIScreenRule.h"

void UEpochUIScreen::NativeConstruct()
{
	Super::NativeConstruct();
	
	for (const TSubclassOf<UEpochUIScreenRule>& RuleClass : OnOpenRules)
	{
		UEpochUIScreenRule* NewInstance = NewObject<UEpochUIScreenRule>(this, RuleClass);
		OnOpenRuleInstances.Add(NewInstance);
	}
	
	for (const TSubclassOf<UEpochUIScreenRule>& RuleClass : OnCloseRules)
	{
		UEpochUIScreenRule* NewInstance = NewObject<UEpochUIScreenRule>(this, RuleClass);
		OnCloseRuleInstances.Add(NewInstance);
	}
}

void UEpochUIScreen::TriggerShow()
{
	for (const TObjectPtr<UEpochUIScreenRule>& OnOpenRule : OnOpenRuleInstances)
	{
		if (OnOpenRule)
			OnOpenRule->Execute();
	}
	
	Show();
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
	
	Hide();
}

void UEpochUIScreen::Hide_Implementation()
{
	BroadcastOnHideFinished();
}

void UEpochUIScreen::BroadcastOnShowFinished() const
{
	OnShowFinished.Broadcast();
}

void UEpochUIScreen::BroadcastOnHideFinished() const
{
	OnHideFinished.Broadcast();
}


