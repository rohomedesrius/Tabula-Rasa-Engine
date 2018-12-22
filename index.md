# Tabula Rasa Engine (Forked)
Tabula Rasa is a simple 3D Game Engine developed for educational purposes. Written in C++ and using OpenGL.
This is a forked version of the engine, [here](https://github.com/Wilhelman/Tabula-Rasa-Engine) you can find the link to the original engine. Our task was to integrate Wwise and use it in our 3D enviroment. We call this sub-system: FirulWwise.
This project has been done by Roger Homedes and Samuel Navarro of Polytechnical University of Catalunya (Terrasa, Spain) for the subject Game Engines on September 2018.

## The Developers

**Roger Homedes**: [github.com/rohomedesrius](https://github.com/rohomedesrius)   
![]()

* FirulWwise main structure
* Main Audio Panel
* Spatial Sound


**Samuel Navarro**: [github.com/GottaCodeHarder](https://github.com/GottaCodeHarder)   

![](https://66.media.tumblr.com/9386a8fd875197d3a5f9655375f24d22/tumblr_pk5tjrlvUt1vonij4o1_400.png)

* Integrated Wwise libraries into Tabula Rasa
* FirulWwise base
* Sound Events
* Background blending music

## Main Sub-System: *FirulWwise*

FirulWwise is our library with the main purpose of integrating [Wwise 2018.2.4](https://www.audiokinetic.com/library/edge/?source=Help&id=welcome_to_wwise) in a fast way.
If you include FirulWwise into your visual studio project, you'll just need to add the following route: "$(ProjectDir)/AK/include" to your debugging directories inside your project properties.



## License  
Copyright 2018 Roger Homedes & Samuel Navarro

Licensed under the [MIT License](https://github.com/rohomedesrius/Tabula-Rasa-Engine/blob/master/LICENSE).

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
