// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class VenlySDK_UE_Beamable : ModuleRules
{
	public VenlySDK_UE_Beamable(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PublicDependencyModuleNames.AddRange(new string[] { "Venly", "VenlyBeamable" });
        PublicDependencyModuleNames.AddRange(new string[] { "BeamableCore", "BeamableCoreRuntime", "Json", "JsonUtilities" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
