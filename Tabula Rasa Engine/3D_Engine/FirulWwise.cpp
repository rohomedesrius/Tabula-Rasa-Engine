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

void FirulWwise::PauseAllEvents()
{
	AK::SoundEngine::PostEvent("Pause_All", AK_INVALID_GAME_OBJECT);
}

void FirulWwise::ResumeAllEvents()
{
	AK::SoundEngine::PostEvent("Resume_All", AK_INVALID_GAME_OBJECT);
}

void FirulWwise::StopAllEvents()
{
	AK::SoundEngine::StopAll();
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
AKEmitter::AKEmitter(const char* _name, AkVector pos, unsigned int _id, int type): e_name(_name), e_pos(pos), e_id(_id), e_type(type)
{
	AKRESULT success = AK::SoundEngine::RegisterGameObj(e_id, e_name);
	if (success != AK_Success)
	{
		assert(!"FirulWwise - Error registering GameObject!");
	}

	SetPosition(pos.X, pos.Y, pos.Z, 1, 0, 0, 0, 1, 0);
}

AKEmitter::~AKEmitter()
{
	AK::SoundEngine::UnregisterGameObj(e_id);
}

void AKEmitter::PlayEvent(const char* name)
{
	AkPlayingID playing_id = AK::SoundEngine::PostEvent(name, e_id);
}

void AKEmitter::PlayEvent(unsigned long id)
{
	AkPlayingID playing_id = AK::SoundEngine::PostEvent(id, e_id);
}


void AKEmitter::StopEvent(const char* name)
{
	AK::SoundEngine::PostEvent(name, AK::SoundEngine::AkActionOnEventType_Stop);
}

void AKEmitter::SetState(const char * state_group, const char * state)
{
	AK::SoundEngine::SetState(state_group, state);
}

void AKEmitter::SetPosition(float pos_x, float pos_y, float pos_z, float orient_front_x, float orient_front_y, float orient_front_z, float orient_top_x, float orient_top_y, float orient_top_z)
{
	AkVector vec_pos;
	vec_pos.X = pos_x;
	vec_pos.Y = pos_y;
	vec_pos.Z = pos_z;

	AkVector vec_ori_front;
	vec_ori_front.X = orient_front_x;
	vec_ori_front.Y = orient_front_y;
	vec_ori_front.Z = orient_front_z;

	AkVector vec_ori_top;
	vec_ori_top.X = orient_top_x;
	vec_ori_top.Y = orient_top_y;
	vec_ori_top.Z = orient_top_z;

	// length = sqrt((ax * ax) + (ay * ay) + (az * az)) 
	float length_front = sqrt(pow(vec_ori_front.X, 2) + pow(vec_ori_front.Y, 2) + pow(vec_ori_front.Z, 2));
	float length_top = sqrt(pow(vec_ori_top.X, 2) + pow(vec_ori_top.Y, 2) + pow(vec_ori_top.Z, 2));

	//Normalize: x = ax/|a|
	vec_ori_front.X = vec_ori_front.X / length_front;
	vec_ori_front.Y = vec_ori_front.Y / length_front;
	vec_ori_front.Z = vec_ori_front.Z / length_front;
	vec_ori_top.X = vec_ori_top.X / length_top;
	vec_ori_top.Y = vec_ori_top.Y / length_top;
	vec_ori_top.Z = vec_ori_top.Z / length_top;

	//Dot product to check if they are orthogonals
	if ((vec_ori_top.X*vec_ori_front.X + vec_ori_top.Y*vec_ori_front.Y + vec_ori_top.Z*vec_ori_front.Z) <= 0)
	{
		AkSoundPosition new_pos;
		new_pos.SetPosition(vec_pos);
		new_pos.SetOrientation(vec_ori_front, vec_ori_top);

		if (this != nullptr)
		{
			e_pos = vec_pos;
			AK::SoundEngine::SetPosition(e_id, new_pos);
		}
	}
}

void AKEmitter::SetSave(bool set)
{
	needs_save = set;
}

bool AKEmitter::NeedsSave()
{
	return needs_save;
}

void AKEmitter::ChangeVolume(const char* game_parameter, int volume)
{
	AkRtpcValue vol_value = volume;
	AK::SoundEngine::SetRTPCValue(game_parameter, vol_value, e_id);
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

AKEmitter* FirulWManager::CreateEmitter(const char* name, AkVector pos, unsigned int id, bool is_listener, int type)
{
	AKEmitter* emitter = new AKEmitter(name, pos, id, type);

	if (id == 0 && is_listener)
	{
		// LISTENER
		AkGameObjectID listener_id = id;
		AK::SoundEngine::SetDefaultListeners(&listener_id, 1);
	}
	else
	{ 
		// EMITTER
		AkGameObjectID emitter_id = id;
		AkGameObjectID go_id = emitter->GetID();

		AK::SoundEngine::SetListeners(emitter_id, &go_id, 2);
	}

	return emitter;
}

void FirulWManager::ChangeVolume(const char* name, float value, AkGameObjectID id)
{
	AK::SoundEngine::SetRTPCValue(name, value, id);
}

