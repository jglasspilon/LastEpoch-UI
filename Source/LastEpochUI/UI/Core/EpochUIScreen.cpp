// Fill out your copyright notice in the Description page of Project Settings.


#include "EpochUIScreen.h"
#include "LastEpochUI/UI/Structs/FScreenInstanceData.h"

void UEpochUIScreen::Show_Implementation()
{
	BroadcastOnShowFinished();
}

void UEpochUIScreen::BroadcastOnShowFinished()
{
	OnShowFinished.Broadcast();
}

void UEpochUIScreen::Hide_Implementation()
{
	BroadcastOnHideFinished();
}

void UEpochUIScreen::BroadcastOnHideFinished()
{
	OnHideFinished.Broadcast();
}