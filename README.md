# Aptos-Cpp-SDK

Aptos-Cpp-SDK is a cpp package written in C++ to help developers integrate Aptos blockchain technology into their cpp and Unreal projects.

- [Features](#features)
- [Requirements](#requirements)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Example Unreal Project](#example-unreal-project)
- [Using Aptos-Cpp-SDK](#using-aptos-cpp-sdk)
  - [RestClient](#restclient)
  - [FaucetClient](#faucetclient)
  - [TokenClient](#tokenclient)
  - [EntryFunction](#entryfunction)
  - [Account](#account)
  - [Wallet](#wallet)
- [Examples](#examples)
- [License](#license)

### Features ###

- [x] Generate new wallets.
- [x] Create new accounts using the ED25519 Key Standard.
- [x] Simulate and submit transaction.
- [x] Create new collections.
- [x] Create new tokens.
- [x] Check account information (Token and APT balances, submitted contracts, etc).
- [x] Import previously used wallets using BIP-39 and BIP-32 Mnemonic seeds or an ED25519 private key.
- [x] Create arbitrary tokens.
- [x] Compatibility with main, dev, and test networks.
- [x] Comprehensive Unit and Integration Test coverage.

### Requirements ###

| Platforms                              | Unreal Version | Installation           | Status       |
| -------------------------------------- | ------------- | ---------------------- | ------------ |
| Mac / Linux                            | Unity engine 5.3| 3rd lib build config| Fully Tested |


### Dependencies
- https://github.com/edwardstock/bip3x
- https://github.com/edwardstock/toolbox
- https://github.com/weidai11/cryptopp

### Installation ###
#Install package management
```
brew install conan
```

#For setup project env
```
conan install . -s compiler.cppstd=20 --build=missing
```

#For build project
```
mkdir build
cd build
cmake ..
```
Note: For MacOS maybe have error when not found path 'macos'. To fix that update file conan_toolchain.cmake like below:
```
set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk CACHE STRING "" FORCE)
```

#For run code coverage on MacOS
First of all, make sure you have llvm and lcov installed. You can install them using brew:
brew install llvm lcov
To generate coverage data, you would then do:
cmake -DCODE_COVERAGE=ON ..
make
make coverage

### Example Unreal Project
A examples unreal project can be found in the following directory:  
`AptosUI/`.

The SDK's test suite can be found in the following directory:   
`Test/`.   

The test suite covers:
- Account - private / public keys, signatures and verification
- Transactions - creation and serialization
- BCS serialization and deserialization

### Using Aptos-Cpp-SDK
Aptos-Cpp-SDK can integrate into your own any cpp or Unreal projects. The main functionality comes from several key classes: `RestClient`, `FacetClient`, `TokenClient`, `EntryFunction`, `Account`, and `Wallet`.

For Unreal project please reference example in ```AptosUI```, you need define Build.cs to integration Aptos library with Unreal engine.
Here is an example:
```cshape
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
			string Bip3xLibPath = Path.Combine(ModuleDirectory, "../../../Plugins/lib/", "libbip3x.dylib");
			string destinationDirectory = Target.ProjectFile.Directory.FullName;
			File.Copy(AptosUiLogicPath, Path.Combine(destinationDirectory, "libAptosUILogic.dylib"), true);
			File.Copy(AptosLibPath, Path.Combine(destinationDirectory, "libAptos.dylib"), true);
			File.Copy(Bip3xLibPath, Path.Combine(destinationDirectory, "libbip3x.dylib"), true);

			PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "../../../") });
			PublicAdditionalLibraries.Add(Path.Combine(destinationDirectory, "libAptos.dylib"));
			PublicAdditionalLibraries.Add(Path.Combine(destinationDirectory, "libAptosUILogic.dylib"));
			PublicAdditionalLibraries.Add(Path.Combine(destinationDirectory, "libbip3x.dylib"));
        }

		PublicIncludePaths.AddRange(new string[] { "/usr/local/include/" });
		bEnableUndefinedIdentifierWarnings = false;
		CppStandard = CppStandardVersion.Cpp17;
	}
}
```

There are three core client classes:
- **FaucetClient** - used to request for airdrops
- **RESTClient** - used to query the aptos blockchain
- **AptosClient** - a REST client wrapper used to interact with Aptos tokens

Let's go over each of the classes, along with examples for each to demonstrate their power and flexibility.
#### RestClient ####
#### FaucetClient ####
#### TokenClient ####
#### EntryFunction ####
#### Account ####
#### Wallet ####

### Examples ###

The SDK comes with several examples that show how to leverage the SDK to its full potential. The examples include `AptosToken`, `Multisig`, `SimulateTransferCoin`, `TransferCoin`, and `SimpleNftExample`.

### License ###

