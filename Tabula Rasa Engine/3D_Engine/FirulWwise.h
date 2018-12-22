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

// Wwise
namespace AK
{

#ifdef WIN32

	void * AllocHook(size_t in_size);
	void FreeHook(void * in_ptr);

	void * VirtualAllocHook(void * in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect);
	void VirtualFreeHook(void * in_pMemAddress,	size_t in_size,	DWORD in_dwFreeType);

#endif
}



// FirulWwise
namespace FirulWwise
{
	//WWise stuff
	bool InitFWw(const wchar_t* banks_directory);
	void CleanUpFWw();
	void ProcessAudio();
}

//EMITTER ===========================================================================================================
class AKEmitter {
public:
	AKEmitter(const char* _name, AkVector pos, unsigned int _id);
	~AKEmitter();
	 
	//Events 
	void PlayEvent(const char* name);
	void PlayEvent(unsigned long id);
	void PauseEvent(const char* name);
	void ResumeEvent(const char* name);
	void StopEvent(const char* name);
	void StopAllEvents();

	//Getter
	AkGameObjectID GetID() { return e_id; }
	bool IsListener() { return listener; }
	bool SetListener(bool set) { return listener = set; }

private:
	AkGameObjectID e_id;
	const char* e_name = nullptr;
	AkVector e_pos;
	bool listener = false;
};

// MANAGER ===========================================================================================================
namespace FirulWManager
{
	bool LoadSoundBank(const char* bank_path);
	bool UnloadSoundBank(const char* bank_name);

	AKEmitter* CreateEmitter(const char* name, AkVector pos, unsigned int id, bool is_listener);
}
#endif