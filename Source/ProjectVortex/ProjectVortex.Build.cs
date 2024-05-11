// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectVortex : ModuleRules
{
	public ProjectVortex(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "PhysicsCore" });
        PublicIncludePaths.AddRange(new string[]
        {
            "ProjectVortex/Character",
            "ProjectVortex/FuncLibrary",
            "ProjectVortex/Game",
            "ProjectVortex/Weapons",
            "ProjectVortex/Structure",
            "ProjectVortex",
        });
    }
}
