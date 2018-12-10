#ifndef __FIRULWWISE_H__
#define __FIRULWWISE_H__

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/SoundEngine/Common/AkTypes.h>

// FirulWwise
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include "AK/samples/Win32/AkFilePackageLowLevelIOBlocking.h"

// Input libraries
#ifndef _DEBUG  // Profile build configuration must be loaded instead of Debug
#define AK_OPTIMIZED
#pragma comment( lib, "AK/libraries/Release(StaticCRT)/lib/AkSoundEngine.lib")
#pragma comment( lib, "AK/libraries/Release(StaticCRT)/lib/AkMusicEngine.lib")
#pragma comment( lib, "AK/libraries/Release(StaticCRT)/lib/AkMemoryMgr.lib")
#pragma comment( lib, "AK/libraries/Release(StaticCRT)/lib/AkStreamMgr.lib")

#else
#include <AK/Comm/AkCommunication.h>
#pragma comment( lib, "AK/libraries/Debug(StaticCRT)/lib/CommunicationCentral.lib" )
#pragma comment( lib, "AK/libraries/Debug(StaticCRT)/lib/AkSoundEngine.lib" )
#pragma comment( lib, "AK/libraries/Debug(StaticCRT)/lib/AkMusicEngine.lib" )
#pragma comment( lib, "AK/libraries/Debug(StaticCRT)/lib/AkMemoryMgr.lib" )
#pragma comment( lib, "AK/libraries/Debug(StaticCRT)/lib/AkStreamMgr.lib" )

#endif

#pragma comment( lib, "AK/libraries/DirectSound/dinput8.lib" )
#pragma comment( lib, "AK/libraries/DirectSound/dsound.lib" ) 
#pragma comment( lib, "AK/libraries/DirectSound/dxguid.lib" )  

namespace FirulWwise
{

	bool InitFWw();
	void CleanUpFWw();

}

#endif //__FIRULWWISE_H__