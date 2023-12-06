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
		string AptosUiLogicPath = Path.Combine(ModuleDirectory, "../../../build/", "libAptosUILogic.dylib");
		string AptosLibPath = Path.Combine(ModuleDirectory, "../../../build/", "libAptos.dylib");
		string destinationDirectory = Target.ProjectFile.Directory.FullName;
		File.Copy(AptosUiLogicPath, Path.Combine(destinationDirectory, "libAptosUILogic.dylib"), true);
		File.Copy(AptosLibPath, Path.Combine(destinationDirectory, "libAptos.dylib"), true);

		PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "../../../") });
		PublicAdditionalLibraries.Add(Path.Combine(destinationDirectory, "libAptosUILogic.dylib"));
		PublicAdditionalLibraries.Add(Path.Combine(destinationDirectory, "libAptos.dylib"));
		//PublicAdditionalLibraries.Add("/usr/local/lib/libbitcoin-system.so");
		//PublicAdditionalLibraries.Add("/usr/local/lib/libcpprest.so");

		PublicIncludePaths.AddRange(new string[] { "/usr/local/include/" });
		//bUseRTTI = true;
		bEnableUndefinedIdentifierWarnings = false;
		CppStandard = CppStandardVersion.Cpp17;
		//PublicDefinitions.Add("_LIBCPP_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION");
	}
}
