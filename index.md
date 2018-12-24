# Tabula Rasa Engine (Forked)
Tabula Rasa is a simple 3D Game Engine developed for educational purposes. Written in C++ and using OpenGL.  
This is a forked version of the engine, [here](https://github.com/Wilhelman/Tabula-Rasa-Engine) you can find the link to the original engine. Our task was to integrate Wwise and use it in our 3D enviroment. We call this sub-system: FirulWwise.
This project has been done by Roger Homedes and Samuel Navarro of Polytechnical University of Catalunya (Terrasa, Spain) for the subject Game Engines on September 2018.
  
---------------------------------------------------------------------------------
## The Developers
---------------------------------------------------------------------------------
  
### **Roger Homedes**: [github.com/rohomedesrius](https://github.com/rohomedesrius)   
![](https://66.media.tumblr.com/35eb54fa29cbcb6ef6c12bd866cbb614/tumblr_pk7gn1xheT1vonij4o1_250.jpg)  

* Created FirulWwise main structure  
  
* Coded FirulWwise AKEmitters  
  
* Coded Main Audio Panel structure  
  
* Coded Spatial Sound with Emitters and demo  
  
* Features tester  

### **Samuel Navarro**: [github.com/GottaCodeHarder](https://github.com/GottaCodeHarder)   

![](https://i.imgur.com/Ed8p3WM.png)  

* Integrated Wwise libraries into Tabula Rasa  
  
* Created FirulWwise base  
  
* Coded Sound Events  
  
* Coded Background blending music with Emitters and demo  
  
* Coded Audio Panel's Emitter Creation  
  
* Memory Leak hunter  

---------------------------------------------------------------------------------
## Main Sub-System: *FirulWwise* 
---------------------------------------------------------------------------------

![](https://i.imgur.com/xp5FnXn.png)  

FirulWwise is our library with the main purpose of integrating [Wwise 2018.2.4](https://www.audiokinetic.com/library/edge/?source=Help&id=welcome_to_wwise) in a fast way.  
If you include FirulWwise into your visual studio project, you'll just need to import the AK folder into your project's folder add the following route: "$(ProjectDir)/AK/include" to your debugging directories inside your project properties.  
And that's it! You're ready to use Wwise!  

You need to call some functions in order to implement FirulWwise, those are:  

* **FirulWwise::InitFWw(wchar_t* banks_directory)** in order to initialize Wwise and the sub-system.  
  
* **FirulWwise::ProcessAudio()** in order to render the audio. Should be called every frame.  
  
* **FirulWwise::CleanUp()** in order to clean Wwise and FirulWwise.  
  

FirulWwise comes with the FirulWManager, which will help you to load and unload soundbanks, and create AKEmitters. AKEmitters are essential to build your scenes, they can be casted as Listeners or Emitters.

---------------------------------------------------------------------------------
## TUTORIAL: Author Demo scene with FirulWwise Walkthrough  
---------------------------------------------------------------------------------

*  Once you run Tabula Rasa press **5** to open the Audio Panel if it's not open by default. This is the tool we've created to implement audio in out engine.  
 
A small window with 3 tabs will pop up. The Soundbanks tab (by default open) allows you to load different Soundbanks, for the demo we will Load both Soundbanks.   
Next, let's move to the "Create Emitters" tab. We can choose from different in-game gameObjects to be set as Emitters or Listeners. Only one Listener is permitted while you can have as much Emitters as possible. If you select the Emitter checkbox you'll notice how several options appear.  
Audio Type lets you choose from Music (blending background music) and SFX (spatial sounds). The Event name must match the event defined in the Soundbank in order to properly function.  
Once you have finished to configure an Emitter o Listener press the "Set" button.  
Follow this steps for a complete Demo scene:  
  
---------------------------------------------------------------------------------
  
First, on the Sounbanks tab:  
* Load **Music.bnk** and **Spatial_Sounds.bnk**.  

![](https://i.imgur.com/wNo37Rm.gif)  
  
Then, on the Create Emitters tab, to create Listeners:  
* Select **Main Camera** and set it as **Listener**. Then press the "Set" button.  
  
To create Emitters:  
* Select **"Music_emitter"** and set it as **Emitter**, select **MUSIC** in Audio Type, set the Duration of each song before blending (we recommend 8 seconds) and press "Set".  
* Select **"Car_emitter"** and set is as Emitter, select **SFX** in Audio Type, and press "Set".  
* Select **"City_emitter_1"** and set it as Emitter, **uncheck** the Demo Spacial Sound Event and manually write **"Play_Spatial_Sound_Animal"** in the Event Name box, then press "Set".   
  
![](https://i.imgur.com/w9CZ82g.gif)
  
Additionally, you can select **"City_emitter_2"** gameobject and write **"Play_Spatial_Sound_Thunder"** in the Event for an extra gameobject emitting sounds.  
  
  
Now, press **Play** button in the top of the window, you can toggle between **Pause/Continue** and **Stop**.  
Warning! Starting and Pausing our Game Engine several times in a short period of time might cause a crash!    
  
* In the Audio Panel, you can modify the different volume parameters to hear different sets of gameobjects, classified by Audio Type. 
  
Check this video if you have troubles following the tutorial!  

[![](https://i.gyazo.com/fa0a8e8d60ad81d5618884d8b308520c.png)](
https://www.youtube.com/watch?v=_SGpB1VT0L8&feature=youtu.be)

You have been shown most of the capabilities of FirulWwise!  
  
---------------------------------------------------------------------------------
## Download the Lastest Release [HERE](https://github.com/rohomedesrius/Tabula-Rasa-Engine/releases/tag/Assignment3.1)

---------------------------------------------------------------------------------
## License  
---------------------------------------------------------------------------------
Copyright 2018 Roger Homedes & Samuel Navarro

Licensed under the [MIT License](https://github.com/rohomedesrius/Tabula-Rasa-Engine/blob/master/LICENSE).

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.  
  