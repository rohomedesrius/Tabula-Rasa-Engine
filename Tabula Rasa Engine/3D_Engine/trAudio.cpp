#include "trAudio.h"
#include "FirulWwise.h"
#include "trDefs.h"

trAudio::trAudio()
{
}

trAudio::~trAudio()
{
}

bool trAudio::Start()
{
	TR_LOG("trAudio: Init with FirulWwise");
	
	const size_t size = strlen(A_SOUNDBANKS_DIR) + 1;
	wchar_t* banks_directory = new wchar_t[size];
	mbstowcs(banks_directory, A_SOUNDBANKS_DIR, size);
	
	FirulWwise::InitFWw(banks_directory);

	return true;
}

bool trAudio::PreUpdate(float dt)
{
	return true;
}

bool trAudio::Update(float dt)
{
	return true;
}

bool trAudio::PostUpdate(float dt)
{
	//FirulWwise::RenderFirul();
	return true;
}
bool trAudio::CleanUp()
{
	TR_LOG("trAudio: CleanUp");

	RELEASE(banks_directory);

	FirulWwise::CleanUpFWw();

	return true;
}

bool trAudio::InitWwise()
{
	return true;
}

bool trAudio::LoadSoundBank(const char* bank_path)
{
	if (bank_path != nullptr)
	{
		std::list<const char*>::const_iterator it = loaded_banks.cbegin();

		while (it != loaded_banks.cend())
		{
			if ((it._Ptr->_Myval) == bank_path)
			{
				TR_LOG("");
				TR_LOG("FirulWwise: SoundBank already Loaded");
				return false;
			}
			it++;
		}

		FirulWManager::LoadSoundBank(bank_path);

		loaded_banks.push_back(bank_path);
	}
	
}
