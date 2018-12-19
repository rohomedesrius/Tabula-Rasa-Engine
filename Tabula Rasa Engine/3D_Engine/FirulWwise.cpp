#include "FirulWwise.h"
#include "trDefs.h"

#include <assert.h>

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

///////////////////////////////////////////
//										 //
//			F I R U L W w I S E			 //
//										 //
///////////////////////////////////////////

bool FirulWwise::InitFWw(const wchar_t* banks_directory)
{
	bool ret = true;

	// Initialize Wwise Memory Manager

	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"Could not create the memory manager.");
		return false;
	}

	// Initialize Wwise Streaming Manager

	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	// Customize the Stream Manager settings here.

	if (!AK::StreamMgr::Create(stmSettings))
	{
		assert(!"Could not create the Streaming Manager");
		return false;
	}   
	
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	// Customize the streaming device settings here.

	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"Could not create the streaming device and Low-Level I/O system");
		return false;
	}

	// Initialize Wwise Sound Engine

	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return false;
	}

	// Initialize Wwise Music Engine

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return false;
	}

	// Initialize Wwise Comunications

#ifndef AK_OPTIMIZED
	
	// Initialize communications (not in release build!)
	/*
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return false;
	}
	*/
#endif // AK_OPTIMIZED

	//Load Banks ----------------------------------------------
	g_lowLevelIO.SetBasePath(banks_directory);
	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

	//Load Init Soundbank (default)------
	FirulWManager::LoadBank("Init.bnk");

	return ret;
}

void FirulWwise::CleanUpFWw()
{

#ifndef AK_OPTIMIZED

	//AK::Comm::Term();			// Terminate Communication Services

#endif
	
	AK::MusicEngine::Term();	// Terminate the music engine
	AK::SoundEngine::Term();	// Terminate the sound engine

	g_lowLevelIO.Term();		// Terminate the streaming device

	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();	 // Terminate the streaming manager

	AK::MemoryMgr::Term();		// Terminate the Memory Manager

}

void FirulWwise::ProcessAudio()
{
	AK::SoundEngine::RenderAudio();
}

// Wwise Related ================================================================================================

#ifdef WIN32

void * AK::AllocHook(size_t in_size)
{
	return malloc(in_size);
}

void AK::FreeHook(void * in_ptr)
{
	free(in_ptr);
}

void * AK::VirtualAllocHook(void * in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect)
{
	return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
}

void AK::VirtualFreeHook(void * in_pMemAddress, size_t in_size, DWORD in_dwFreeType)
{
	VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
}

#endif

// EMITTER =======================================================================================================
AKEmitter::AKEmitter(const char* _name, unsigned int _id): e_name(_name), e_id(_id)
{
	e_pos.Zero();
	AK::SoundEngine::RegisterGameObj(e_id, e_name);
}

AKEmitter::~AKEmitter()
{
	AK::SoundEngine::UnregisterGameObj(e_id);
}

void AKEmitter::PlayEvent()
{

}

void AKEmitter::StopEvent()
{

}

// EVENTS =======================================================================================================

AKEvent::AKEvent()
{
}

AKEvent::~AKEvent()
{
}

// MANAGER ======================================================================================================

bool FirulWManager::LoadBank(const char* bank_path)
{
	AkBankID bank_id;
	AKRESULT success = AK::SoundEngine::LoadBank(bank_path, AK_DEFAULT_POOL_ID, bank_id);
	
	if (!success)
		return false;

	return true;
}