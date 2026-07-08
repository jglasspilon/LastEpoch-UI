#pragma once

#include "IDetailCustomization.h"

class FEpochUIScreenCustomization : public IDetailCustomization
{
	
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	TArray<TSharedPtr<FName>> AnimationOptions;
	
	void BuildAnimationDropdown(IDetailLayoutBuilder& DetailBuilder, const FName PropertyName);
	TArray<FName> GetAnimationNames(UObject* WidgetObject) const;
	
};
