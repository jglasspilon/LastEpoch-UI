#include "EpochUIScreenCustomization.h"
#include "EpochUIScreen.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "PropertyHandle.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"

TSharedRef<IDetailCustomization> FEpochUIScreenCustomization::MakeInstance()
{
    return MakeShareable(new FEpochUIScreenCustomization);
}

void FEpochUIScreenCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    BuildAnimationDropdown(DetailBuilder, GET_MEMBER_NAME_CHECKED(UEpochUIScreen, ShowAnimationName));
    BuildAnimationDropdown(DetailBuilder, GET_MEMBER_NAME_CHECKED(UEpochUIScreen, HideAnimationName));
}

void FEpochUIScreenCustomization::BuildAnimationDropdown(IDetailLayoutBuilder& DetailBuilder, const FName PropertyName)
{
    TSharedPtr<IPropertyHandle> PropertyHandle = DetailBuilder.GetProperty(PropertyName, UEpochUIScreen::StaticClass());

    if (!PropertyHandle.IsValid())
        return;

    // Get the actual widget instance being customized
    TArray<UObject*> OuterObjects;
    PropertyHandle->GetOuterObjects(OuterObjects);

    if (OuterObjects.Num() == 0)
        return;

    UEpochUIScreen* Screen = Cast<UEpochUIScreen>(OuterObjects[0]);
    if (!Screen)
        return;

    // Build animation name list
    TArray<FName> AnimationNames = GetAnimationNames(Screen);

    AnimationOptions.Empty();
    for (const FName& Name : AnimationNames)
    {
        AnimationOptions.Add(MakeShared<FName>(Name));
    }

    // Hide the original property so we can fully control the row
    DetailBuilder.HideProperty(PropertyHandle);

    // Put our custom row in the same category
    IDetailCategoryBuilder& Category = DetailBuilder.EditCategory("Screen");

    Category.AddCustomRow(PropertyHandle->GetPropertyDisplayName())
    .NameContent()
    [
        PropertyHandle->CreatePropertyNameWidget()
    ]
    .ValueContent()
    .MinDesiredWidth(250)
    [
        SNew(SComboBox<TSharedPtr<FName>>)
        .OptionsSource(&AnimationOptions)
        .OnGenerateWidget_Lambda([](TSharedPtr<FName> Item)
        {
            return SNew(STextBlock).Text(FText::FromName(*Item));
        })
        .OnSelectionChanged_Lambda([PropertyHandle](TSharedPtr<FName> NewValue, ESelectInfo::Type)
        {
            if (NewValue.IsValid())
            {
                PropertyHandle->SetValue(*NewValue);
            }
        })
        .Content()
        [
            SNew(STextBlock)
            .Text_Lambda([PropertyHandle]()
            {
                FName CurrentValue;
                PropertyHandle->GetValue(CurrentValue);
                return FText::FromName(CurrentValue);
            })
        ]
    ];
}

TArray<FName> FEpochUIScreenCustomization::GetAnimationNames(UObject* WidgetObject) const
{
    TArray<FName> Names;
    Names.Add(NAME_None);

    UEpochUIScreen* Screen = Cast<UEpochUIScreen>(WidgetObject);
    if (!Screen)
        return Names;

    Screen->CacheAnimations();

    for (const auto& Pair : Screen->GetAnimations())
    {
        Names.Add(Pair.Key);
    }

    return Names;
}
