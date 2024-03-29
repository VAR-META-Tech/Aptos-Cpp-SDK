// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class AptosUI : ModuleRules
{
	public AptosUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnableExceptions = true;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		PrivateDependencyModuleNames.AddRange(new string[] { });
		if (Target.Platform == UnrealTargetPlatform.Mac)
        {
			string AptosUiLogicPath = Path.Combine(ModuleDirectory, "../../../build/", "libAptosUILogic.dylib");
			string AptosLibPath = Path.Combine(ModuleDirectory, "../../../build/", "libAptos.dylib");
			// string Bip3xLibPath = Path.Combine(ModuleDirectory, "../../../Plugins/lib/", "libbip3x.dylib");
			string destinationDirectory = Target.ProjectFile.Directory.FullName;
			File.Copy(AptosUiLogicPath, Path.Combine(destinationDirectory, "libAptosUILogic.dylib"), true);
			File.Copy(AptosLibPath, Path.Combine(destinationDirectory, "libAptos.dylib"), true);
			// File.Copy(Bip3xLibPath, Path.Combine(destinationDirectory, "libbip3x.dylib"), true);

			PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "../../../") });
			PublicAdditionalLibraries.Add(Path.Combine(destinationDirectory, "libAptos.dylib"));
			PublicAdditionalLibraries.Add(Path.Combine(destinationDirectory, "libAptosUILogic.dylib"));
			// PublicAdditionalLibraries.Add(Path.Combine(destinationDirectory, "libbip3x.dylib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
			string AptosUiLogicPath = Path.Combine(ModuleDirectory, "../../../build/", "libAptosUILogic.so");
			string AptosLibPath = Path.Combine(ModuleDirectory, "../../../build/", "libAptos.so");
			string destinationDirectory = Path.Combine(Target.ProjectFile.Directory.FullName, "Binaries/Linux/");
			File.Copy(AptosUiLogicPath, Path.Combine(destinationDirectory, "libAptosUILogic.so"), true);
			File.Copy(AptosLibPath, Path.Combine(destinationDirectory, "libAptos.so"), true);

			PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "../../../") });
			PublicAdditionalLibraries.Add(Path.Combine(destinationDirectory, "libAptosUILogic.so"));
			PublicAdditionalLibraries.Add(Path.Combine(destinationDirectory, "libAptos.so"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Win64) // Assuming 64-bit Windows
		{
			string AptosUiLogicPath = Path.Combine(ModuleDirectory, "../../../build/Release", "AptosUILogic.dll");
			string AptosLibPath = Path.Combine(ModuleDirectory, "../../../build/Release", "Aptos.dll"); 
			string AptosUiLogic_LPath = Path.Combine(ModuleDirectory, "../../../build/Release", "AptosUILogic.lib");
            string AptosLib_LPath = Path.Combine(ModuleDirectory, "../../../build/Release", "Aptos.lib");
            string destinationDirectory = Target.ProjectFile.Directory.FullName;
            string destinationBinaryDirectory = destinationDirectory + "/Binaries/Win64";

            File.Copy(AptosUiLogicPath, Path.Combine(destinationDirectory, "AptosUILogic.dll"), true);
			File.Copy(AptosLibPath, Path.Combine(destinationDirectory, "Aptos.dll"), true);
            File.Copy(AptosUiLogicPath, Path.Combine(destinationBinaryDirectory, "AptosUILogic.dll"), true);
            File.Copy(AptosLibPath, Path.Combine(destinationBinaryDirectory, "Aptos.dll"), true);
            File.Copy(AptosUiLogic_LPath, Path.Combine(destinationDirectory, "AptosUILogic.lib"), true);
            File.Copy(AptosLib_LPath, Path.Combine(destinationDirectory, "Aptos.lib"), true);

            PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "../../../") });
			PublicAdditionalLibraries.Add(Path.Combine(destinationDirectory, "Aptos.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(destinationDirectory, "AptosUILogic.lib"));
			PublicDelayLoadDLLs.Add(Path.Combine(destinationDirectory, "Aptos.dll"));
            PublicDelayLoadDLLs.Add(Path.Combine(destinationDirectory, "AptosUILogic.dll"));

        }

		PublicIncludePaths.AddRange(new string[] { "/usr/local/include/" });
		//bUseRTTI = true;
		bEnableUndefinedIdentifierWarnings = false;
		CppStandard = CppStandardVersion.Cpp17;
		//PublicDefinitions.Add("_LIBCPP_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION");
	}
}
