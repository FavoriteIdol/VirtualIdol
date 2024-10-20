// VRM4U Copyright (c) 2021-2024 Haruyoshi Yamamoto. This software is released under the MIT License.

using UnrealBuildTool;

public class VRM4UCapture : ModuleRules
{
	public VRM4UCapture(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        //PublicDefinitions.Add("VRM4UCAPTURE_API=DLLEXPORT");

        PublicDependencyModuleNames.AddRange(
			new string[] {
				"OSC",
				"Core",
				"CoreUObject",
                "Engine",
                "Networking",
                "Sockets",
				"AnimGraphRuntime",

				"VRM4U",
				"VirtualIdol"
            });

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.Add("UnrealEd");
		}


		PrivateIncludePathModuleNames.AddRange(
			new string[] {
            });

		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
			});

        PrivateIncludePaths.AddRange(
        new string[] {
			// Relative to Engine\Plugins\Runtime\Oculus\OculusVR\Source
			//"../Runtime/Renderer/Private",
        });
    }
}
