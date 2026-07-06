#pragma once
#include "GameplayTagContainer.h"
#include "FScreenInstanceData.generated.h"

class UEpochUIScreen;

USTRUCT()
struct FScreenInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UEpochUIScreen> ScreenTypeToInstantiate;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag OwningLayer;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag ScreenName;
};
