#include "trAudio.h"
#include "FirulWwise.h"
#include "trDefs.h"
#include "GameObject.h"

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
	FirulWwise::ProcessAudio();

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
	return true;
}

bool trAudio::UnloadSoundBank(const char* bank_path)
{
	if (bank_path != nullptr)
	{
		std::list<const char*>::const_iterator it = loaded_banks.cbegin();

		while (it != loaded_banks.cend())
		{
			if ((it._Ptr->_Myval) == bank_path)
			{
				TR_LOG("");
				loaded_banks.erase(it);
				FirulWManager::UnloadSoundBank(bank_path);
				TR_LOG("FirulWwise: %s SoundBank Unloaded", bank_path);
				return false;
			}
			it++;
		}
	}
	TR_LOG("");
	TR_LOG("FirulWwise: %s SoundBank not found", bank_path);
}

AKEmitter* trAudio::CreateEmitter(const char* name, GameObject* go, bool is_listener)
{
	AKEmitter* emitter;

	AkVector pos;
	pos.Zero();
	pos.X = go->GetTransform()->GetTranslation().x;
	pos.Y = go->GetTransform()->GetTranslation().y;
	pos.Z = go->GetTransform()->GetTranslation().z;

	if (firul_emitters.empty())
	{
		emitter = FirulWManager::CreateEmitter(name, pos, 0, is_listener);
	}
	else
	{
		unsigned int id = firul_emitters.back()->GetID();

		emitter = FirulWManager::CreateEmitter(name, pos, id + 1, is_listener);
	}

	firul_emitters.push_back(emitter);

	return emitter;
}