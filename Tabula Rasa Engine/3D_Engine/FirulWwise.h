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

	void PauseAllEvents();
	void ResumeAllEvents();
	void StopAllEvents();
}

//EMITTER ===========================================================================================================
class AKEmitter {
public:
	AKEmitter(const char* _name, AkVector pos, unsigned int _id, int type);
	~AKEmitter();
	 
	//Events 
	void PlayEvent(const char* name);
	void PlayEvent(unsigned long id);
	void StopEvent(const char* name);
	void SetState(const char* state_group, const char* state);
	
	//Getter
	AkGameObjectID GetID() { return e_id; }
	const char* GetName() { return e_name; }
	bool IsListener() { return listener; }
	int GetType() { return e_type; }
	//Setter
	bool SetListener(bool set) { return listener = set; }
	void SetPosition(float pos_x, float pos_y, float pos_z, float orient_front_x, float orient_front_y, float orient_front_z, float orient_top_x, float orient_top_y, float orient_top_z);

	float GetVolume() const;
	void ChangeVolume(const char* game_parameter, int volume, bool master);

	void ApplyAuxBus(const char* aux_bus_name, AkReal32 value, AkGameObjectID id);

private:
	AkGameObjectID e_id;
	const char* e_name = nullptr;
	AkVector e_pos;
	bool listener = false;
	int e_type = 2;

	float volume = 50;
};

// MANAGER ===========================================================================================================
namespace FirulWManager
{
	bool LoadSoundBank(const char* bank_path);
	bool UnloadSoundBank(const char* bank_name);

	AKEmitter* CreateEmitter(const char* name, AkVector pos, unsigned int id, bool is_listener, int type);

	void ChangeVolume(const char* name, float value, AkGameObjectID id);
}
#endif