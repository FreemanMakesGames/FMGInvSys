# FMGInvSys
FMG Inventory System, or FMGInvSys, is an Unreal Engine plugin, that features uniquely flexible item combinations and item widget setup.

# Demo
You can find demo videos [here](https://www.freemanmakesgames.pro/work/fmg-inv-sys).

# Installation
Either download or git-clone the repository. You'll get a directory named `FMGInvSys-master`. Then at the root directory of your project, create a new directory named `Plugins`, if it's not already there. Put `FMGInvSys-master` under the `Plugins` directory. The path to the plugin should be: `X:/.../<ProjectName>/Plugins/FMGInvSys-master`.

You may need to re-generate Visual Studio project files, for it to show up in your source code editor.

Then in your `<ProjectName>.Build.cs`, append `"FMGInvSys"` in this line of code:
```cpp
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
```
This adds the module to your project.

Currently the plugin supports UE 4.25.

# Usage
The docs are in the [Wiki](https://github.com/FreemanMakesGames/FMGInvSys/wiki).

# Getting Help
For now, GitHub Issues seems to be a good place to ask your questions, in addition to submitting bug reports and suggestions.

More support may be offered when the project's first published, as there's likely going to be a lot of issues and needs for improvement. As the project gains more audience, and meanwhile grows to be more robust, the amount of support provided per user may gradually decline.
