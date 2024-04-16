// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BossHunter : ModuleRules
{
	public BossHunter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "AIModule", "NavigationSystem" });
	}
}
