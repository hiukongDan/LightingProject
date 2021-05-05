# LightingProject
A School OpengGL Projoect
## Building Instruction
- click cmd.bat
- create a build folder "mkdir build" and change into it
- run in command prompt "cmake -G 'Visual Studio 15 Win64' .."
- building in Visual Studio 2017 Community 15.9.2 (on author's machine)
## After building
- copy assimp and nanogui's dlls into build/Debug/
- run visual studio building again for "LightingProject" project

## Features
- loading one obj model
- change lighting model from "gauroud, phong, blinn"
- change lighting color
- only one point light

## Softwares required
- Visual Studio 2017
- Cmake
