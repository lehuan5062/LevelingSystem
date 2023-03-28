// Copyright 2023 Lê Vương Gia Huân

using UnrealBuildTool;

public class LevelingSystem : ModuleRules
{
	public LevelingSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Engine",
                "CurrencySystem"
            }
            );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "DeveloperSettings"
            }
            );
    }
}
