#ifndef __WWISE_INCLUDE_H__
#define __WWISE_INCLUDE_H__

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


// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
namespace AK
{

#ifdef WIN32

	void * AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}

	void FreeHook(void * in_ptr)
	{
		free(in_ptr);
	}

	// Note: VirtualAllocHook() may be used by I/O pools of the default implementation
	// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
	// This is NOT mandatory; you may implement it with a simple malloc().
	void * VirtualAllocHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
	)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}

	void VirtualFreeHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
	)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
#endif

}

#endif