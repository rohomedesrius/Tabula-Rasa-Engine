#include "FirulWwise.h"
#include "trDefs.h"
#include "trLog.h"

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
	FirulWManager::LoadSoundBank("Init.bnk");

	return ret;
}

void FirulWwise::CleanUpFWw()
{
	// Unregister GameObjects
	AK::SoundEngine::UnregisterAllGameObj();

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
AKEmitter::AKEmitter(const char* _name, AkVector pos, unsigned int _id): e_name(_name), e_pos(pos), e_id(_id)
{
	AKRESULT success = AK::SoundEngine::RegisterGameObj(e_id, e_name);
	if (success != AK_Success)
	{
		assert(!"FirulWwise - Error registering GameObject!");
	}
}

AKEmitter::~AKEmitter()
{
	AK::SoundEngine::UnregisterGameObj(e_id);
}

void AKEmitter::PlayEvent()
{
	AkGameObjectID game_obj = 3;
	AK::SoundEngine::RegisterGameObj(game_obj);
	
	AkPlayingID playing_id = AK::SoundEngine::PostEvent(L"Play_Sound_01", game_obj);
}

void AKEmitter::PlayEvent(unsigned long id)
{
	AK::SoundEngine::RegisterGameObj(id);

	AkPlayingID playing_id = AK::SoundEngine::PostEvent(L"Play_Sound_01", id);
}

void AKEmitter::PauseEvent(const char * name)
{
	AK::SoundEngine::ExecuteActionOnEvent(name, AK::SoundEngine::AkActionOnEventType_Pause);
}

void AKEmitter::ResumeEvent(const char * name)
{
	AK::SoundEngine::ExecuteActionOnEvent(name, AK::SoundEngine::AkActionOnEventType_Resume);
}


void AKEmitter::StopEvent(const char* name)
{
	AK::SoundEngine::ExecuteActionOnEvent(name, AK::SoundEngine::AkActionOnEventType_Stop);
}

// MANAGER ======================================================================================================

bool FirulWManager::LoadSoundBank(const char* bank_path)
{
	AkBankID bank_id;
	AKRESULT success = AK::SoundEngine::LoadBank(bank_path, AK_DEFAULT_POOL_ID, bank_id);
	
	if (success == AK_Fail)
	{
		assert(!"FirulWwise: Couldn't load %s SoundBank", bank_path);
		return false;
	}

	//TR_LOG("FirulWwise: %s SoundBank Loaded", bank_path);
}

bool FirulWManager::UnloadSoundBank(const char* bank_name)
{
	AKRESULT success = AK::SoundEngine::UnloadBank( bank_name, NULL);
	
	if (success == AK_Fail)
	{
		assert(!"FirulWwise: Couldn't unload %s SoundBank", bank_name);
		return false;
	}

	return true;
}

AKEmitter* FirulWManager::CreateEmitter(const char* name, AkVector pos, unsigned int id, bool is_listener)
{
	AKEmitter* emitter = new AKEmitter(name, pos, id);

	if (id == 0 && is_listener)
	{
		AkGameObjectID listener_id = id;
		AK::SoundEngine::SetDefaultListeners(&listener_id, 1);
	}
	else
	{
		AkGameObjectID listener_id = id;
		
		//AK::SoundEngine::SetListeners(listener_id, emitter->GetID());
	}

	return emitter;
}