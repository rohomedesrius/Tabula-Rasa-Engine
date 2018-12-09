#include "FirulWwise.h"
#include "WwiseInclude.h"
#include <assert.h>

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

bool FirulWwise::InitFWw()
{
	bool ret = true;

	// Create and initialize an instance of the default memory manager. Note
	// that you can override the default memory manager with your own. Refer
	// to the SDK documentation for more information.

	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"Could not create the memory manager.");
		return false;
	}

	// Create and initialize an instance of the default streaming manager. Note
	// that you can override the default streaming manager with your own.

	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	// Customize the Stream Manager settings here. <<========

	if (!AK::StreamMgr::Create(stmSettings))
	{
		assert(!"Could not create the Streaming Manager");
		return false;
	}

	// Create a streaming device with blocking low-level I/O handshaking.
	// Note that you can override the default low-level I/O module with your own. Refer
	// to the SDK documentation for more information.      
	
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	// Customize the streaming device settings here.

	// CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
	// in the Stream Manager, and registers itself as the File Location Resolver.
	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"Could not create the streaming device and Low-Level I/O system");
		return false;
	}

	// Initialize the sound engine
	// Using default initialization parameters

	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return false;
	}

	// Initialize the music engine
	// Using default initialization parameters

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return false;
	}

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
