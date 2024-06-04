// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Spectrum : ModuleRules
{
	public Spectrum(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "Spectrum" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "SlateCore", "Slate", "Sockets", "Networking",
	        "EnhancedInput", "UMG", "GeometryCollectionEngine","GeometryFramework", "AIModule",
	        "MultiplayerSessions","OnlineSubsystem","OnlineSubsystemSteam", "NavigationSystem","GameplayTasks"
        });
         
		PrivateDependencyModuleNames.AddRange(new string[] { "ProtobufCore" });

        PrivateIncludePaths.AddRange(new string[]
{
            "Spectrum/",
            "Spectrum/Network/",
            "Spectrum/Game/",
});

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
