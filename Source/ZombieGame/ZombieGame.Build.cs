// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;



public class ZombieGame : ModuleRules
{
	public ZombieGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "GameplayTasks", "PhysicsCore", "Niagara", "AIModule", "NavigationSystem" });

		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "/Engine/Plugins/FX/Niagara/Source"));

		
	}
}
