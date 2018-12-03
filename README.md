﻿# Tabula Rasa Engine
A 3d game engine developed with OpenGL in c ++. This project has been done by two students of CITM - UPC (Terrasa, Spain) for the subject Game Engines, 

* [Github repository](https://github.com/Wilhelman/Tabula-Rasa-Engine)
* [Website] NOT YET
* [Wiki] NOT YET

## Installation instructions

Download the zip file and unzip it. Open the folder, execute the .exe and enjoy!
You can find the models and textures in the _assets_ folder. All models have a _DDS_ texture associated.

_IMPORTANT: do not modify, change or add any folder or file as you might not be able to execute the game engine._

## Controls

* **Select object:** Left click

### Gizmos

* **W/E/R:** Change gizmo to Translate, Rotation and Scale

### Assets controls

* **Load scene:** Double left clic

### Camera

* **Free look around:** Right Click Mouse Button
* **Orbit around object:** LALT + Left Click Mouse Button
* **Zoom in/out** Mouse wheel
* **Pan**: Middle mouse button
* **Duplicate movement speeed**: Hold Shift key
* **Focus camera on object**: F key

### UI 
* **Open config window**: 4
* **Open console**: 1 
* **Open Inspector**: I key
* **Show/Hide UI:** LALT + G key
* **Quit application:** ESC / Alt+F4

_Click again on Window->(any option) to close that window_

## Innovations / Redesign

* Draw the z-buffer in grayscale option determined by the distance (Option in Config window -> Renderer3D options -> Switch Z- Buffer Visualization).

**_Warning: If this option is activated the overall performance might decrease significantly._**

Example for comparison:
![image](https://user-images.githubusercontent.com/25589509/46920311-ccccb780-cfec-11e8-9afa-fcbf64616042.png)


## Team members

_García Subirana, Guillermo's responsability and Github account_
* Code
* [Github account](https://github.com/Wilhelman)

_Masó Garcia, Victor's responsability and Github account_
* Code
* [Github account](https://github.com/nintervik)

## Tools used
* IDE: Microsoft Visual Studio 2017
* Language: C++
* Containers: STL
* Input and audio: SDL 2.0.8
* Graphics: OpenGL
* Math: MathGeoLib 1.5
* Random Number Generator: PCG (C Implementantion) 0.94
* GUI: Deat ImGui 1.65
* 3D Model importer: Assimp 1.4.0
* Image loader: Developer's Image Library (DevIL) 1.8.0
* Data persistance: JSON parser - Parson 2017
* Profiler: Brofiler 1.1.2
* Memory manager: mmgr
* File system: PHYSFS 3.0.1
* Code repository: GitHub
* Others: Adobe Photoshop CS6 / MS Paint / Aseprite

## Credits for models and textures:
* Tank gladiator - Autor: kotbfg | [Link](https://sketchfab.com/models/bc67044a11a64a8c8814014c4b87cc48)
* Forest house - Autor: Wesai | [Link](https://sketchfab.com/models/5646e6a3c8834022a0e289465f0bbc5d)
