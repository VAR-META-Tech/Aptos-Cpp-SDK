// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class AptosUI : ModuleRules
{
	public AptosUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnableExceptions = true;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		PrivateDependencyModuleNames.AddRange(new string[] { });
		PublicIncludePaths.AddRange(new string[] { "/home/nodoka/Documents/Aptos-Cpp-SDK/" });
		PublicAdditionalLibraries.Add("/home/nodoka/Documents/Aptos-Cpp-SDK/build/libAptosUILogic.so");
        PublicAdditionalLibraries.Add("/home/nodoka/Documents/Aptos-Cpp-SDK/build/libAptos.so");
		PublicAdditionalLibraries.Add("/usr/local/lib/libbitcoin-system.so");
		PublicAdditionalLibraries.Add("/usr/local/lib/libcpprest.so");

		PublicIncludePaths.AddRange(new string[] { "/usr/local/include/" });
		//bUseRTTI = true;
		bEnableUndefinedIdentifierWarnings = false;		
		CppStandard = CppStandardVersion.Cpp17;
		//PublicDefinitions.Add("_LIBCPP_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION");
	}
}
