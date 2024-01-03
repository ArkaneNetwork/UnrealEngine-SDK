// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class VenlySDK_UE_BeamableTarget : TargetRules
{
	public VenlySDK_UE_BeamableTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		//CppStandard = CppStandardVersion.Cpp17;

		ExtraModuleNames.AddRange( new string[] { "VenlySDK_UE_Beamable", "VenlySDK_UE_Beamable_MicroservicesBP" } );
        ExtraModuleNames.AddRange(new[] { "BeamableCore", "BeamableCoreRuntime", "BeamableCoreBlueprintNodes" });
    }
}
