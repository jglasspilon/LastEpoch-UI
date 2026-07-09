// Fill out your copyright notice in the Description page of Project Settings.

#include "EpochUIManagementSubsystem.h"
#include "EpochUILayout.h"
#include "Blueprint/UserWidget.h"
#include "LastEpochUI/LastEpochUI.h"
#include "LastEpochUI/UI/DevSettings/EpochUISettings.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void UEpochUIManagementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	const UEpochUISettings* UISettings = GetDefault<UEpochUISettings>();
	UISettings->UILayoutWidget.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &ThisClass::OnUILayoutLoaded));
	UISettings->UIInputMappingContext.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &ThisClass::OnMappingContextLoaded));
}

void UEpochUIManagementSubsystem::OnUILayoutLoaded(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject)
{
	if (!LoadedObject)
	{
		UE_LOG(LogGame, Error, TEXT("Failed to load UI Layer widget class. Loaded empty asset"));
		return;
	}
	
	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
	
	if (!PC)
	{
		UE_LOG(LogGame, Error, TEXT("Failed to load UI Layer widget class. No player controller available for UI creation"));
		return;
	}
	
	UClass* WidgetClass = Cast<UClass>(LoadedObject);
	UILayoutInstance = CreateWidget<UEpochUILayout>(PC, WidgetClass);
	
	if (!UILayoutInstance)
	{
		UE_LOG(LogGame, Error, TEXT("Failed to load UI Layer widget class. Loaded asset was not of type UEpochUILayout."));
		return;
	}
	
	UILayoutInstance->AddToViewport();
	BindInputs(PC);
}

void UEpochUIManagementSubsystem::OnMappingContextLoaded(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject)
{
	if (!LoadedObject)
	{
		UE_LOG(LogGame, Error, TEXT("Failed to load Input Mapping Context. Loaded empty asset"));
		return;
	}
	
	UInputMappingContext* MappingContext = Cast<UInputMappingContext>(LoadedObject);
	if (!MappingContext)
	{
		UE_LOG(LogGame, Error, TEXT("Failed to load Input Mapping Context. Wrong object type loaded."));
		return;
	}

	const ULocalPlayer* LP = GetGameInstance()->GetLocalPlayerByIndex(0);
	if (!LP)
	{
		UE_LOG(LogGame, Error, TEXT("Failed to load Input Mapping Context. No Local Player found to add mapping context to."));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!Subsystem)
	{
		UE_LOG(LogGame, Error, TEXT("Failed to load Input Mapping Context. Enhance Input Local Player Subsystem not found."));
		return;
	}

	Subsystem->AddMappingContext(MappingContext, 1);
}

void UEpochUIManagementSubsystem::BindInputs(const APlayerController* PC)
{
	const UEpochUISettings* UISettings = GetDefault<UEpochUISettings>();
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PC->InputComponent);
	
	for (const TPair<TSoftObjectPtr<UInputAction>, FGameplayTag>& InputMap : UISettings->ScreenInputActionMap)
	{
		const FGameplayTag ScreenKey = InputMap.Value;
		UInputAction* InputAction = InputMap.Key.LoadSynchronous();
		
		if (!InputAction)
			continue;
		
		ScreenInputActionMap.Add(InputAction, ScreenKey);
		EnhancedInput->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::HandleScreenInput);
	}
}

void UEpochUIManagementSubsystem::HandleScreenInput(const FInputActionInstance& Instance)
{
	const UInputAction* Action = Instance.GetSourceAction();
	UILayoutInstance->ToggleScreen(ScreenInputActionMap.FindRef(Action));
}
