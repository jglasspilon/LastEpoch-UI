using UnrealBuildTool;

public class LastEpochUIEditor : ModuleRules
{
    public LastEpochUIEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "UMG",
                "LastEpochUI"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Slate",
                "SlateCore",
                "EditorFramework",
                "UnrealEd",
                "PropertyEditor",
                "InputCore",
                "ApplicationCore"
            }
        );
        
        PublicIncludePaths.AddRange(
            new string[]
        {
            "LastEpochUI/UI/Core",
        });
    }
}