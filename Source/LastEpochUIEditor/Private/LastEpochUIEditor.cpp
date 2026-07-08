#include "LastEpochUIEditor.h"

#include "LastEpochUIEditor/UI/EpochUIScreenCustomization.h"

#define LOCTEXT_NAMESPACE "FLastEpochUIEditorModule"

void FLastEpochUIEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditor.RegisterCustomClassLayout("EpochUIScreen",FOnGetDetailCustomizationInstance::CreateStatic(&FEpochUIScreenCustomization::MakeInstance));
}

void FLastEpochUIEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FLastEpochUIEditorModule, LastEpochUIEditor)