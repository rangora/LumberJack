// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

namespace UnrealBuildTool.Rules
{
	public class MyClient : ModuleRules
	{
			
		private string ThirdPartyPath
		{
			get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty/")); }
		}

		public MyClient(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
			bUseRTTI = true;
			bEnableExceptions = true;

			PublicIncludePaths.AddRange(
				new string[]
				{
					Path.Combine(ModuleDirectory, "public"),
				});


			PrivateIncludePaths.AddRange(
				new string[] 
				{
					Path.Combine(ModuleDirectory, "private"),
					Path.Combine(ThirdPartyPath, "asio/include"),
				});


			PublicDependencyModuleNames.AddRange(
				new string[]
				{
				"Core",
				});


			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
				"CoreUObject",
				"Engine",
				});


			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
				});
		}
	}
}