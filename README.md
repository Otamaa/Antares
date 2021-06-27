# Antares

[![Github All Releases](https://img.shields.io/github/downloads/Phobos-developers/Antares/total.svg)](https://github.com/Phobos-developers/Antares/releases)
[![Workflow](https://img.shields.io/github/workflow/status/Phobos-developers/Antares/Automatic%20Build.svg)](https://github.com/Phobos-developers/Antares/actions)
[![license](https://img.shields.io/github/license/Phobos-developers/Antares.svg)](https://www.gnu.org/licenses/gpl-3.0.en.html)

Antares is a WIP open-source reimplementation of newer closed-source [Ares](https://github.com/Ares-Developers/Ares) versions. This project exists because source code of versions past 0.A are kept private, which means they can't be fixed and Ares can't be contributed to. The aim of that project is to reimplement those and keep it open-source so people can contribute to it.

At the moment the project is not a priority, so we're looking for more contributors. Contact us at our [channel on C&C Mod Haven](https://discord.gg/v4puUfdzed).

Building and Usage
------------------

0. Install **Visual Studio** (2019 is recommended, 2017 is minimum) with the dependencies listed in `.vswhere` (it will prompt you to install missing dependences when you open the project, or you can run VS installer and import the config). If you prefer to use **Visual Studio Code** you may install **VS Build Tools** with the stuff from `.vswhere` instead. You can also don't use any code editor or IDE and build via **command line scripts** included with the project.
1. Clone this repo recursively via your favorite git client (that will also clone YRpp).
2. To build the extension:
  - in Visual Studio: open the solution file in VS and build it (`Debug` build config is recommended);
  - in VSCode: open the project folder and hit `Run Build Task...` (`Ctrl + Shift + B`);
  - barebones: run `scripts/build_debug.bat`.
3. Upon build completion place the resulting `Antares.dll` from folder named identical to the used build config in your YR directory and launch Syringe targeting your YR executable (usually `gamemd.exe`). **Make sure that `Ares.dll` is NOT present, as Antares by nature is incompatible with Ares!**

You can also get an automatic build. There are two ways to do that.
- **Get an artifact via nightly.link**. This is a service that allows guests to download automatic builds from GitHub. You can get a build for the latest successful (marked with a green tick) `master` branch commit via [this link](https://nightly.link/Phobos-developers/Antares/blob/master/.github/workflows/build.yml), or get a build for any up-to-date pull request via an automatic comment that would appear in it.
- **Get an artifact manually from GitHub Actions runs**. You can get an artifact for a specific commit which is built automatically with a GitHub Actions workflow, just press on a green tick, open the workflow, find and download the build artifact. This is limited to authorized users only.

Documentation
-------------
The documentation should be identical to Ares one that can be found at [here @ ares-developers.github.io](https://ares-developers.github.io/Ares-docs/).

Credits
-------
**Ares developer team** - creation of Ares
**AlexB** - development of closed-source Ares past 0.A

Legal and License
-----------------

The Antares project is an unofficial open-source community collaboration project to reverse-engineer the Ares engine extension.

This project has no direct affiliation with Electronic Arts Inc. Command & Conquer, Command & Conquer Red Alert 2, Command & Conquer Yuri's Revenge are registered trademarks of Electronic Arts Inc. All Rights Reserved.

