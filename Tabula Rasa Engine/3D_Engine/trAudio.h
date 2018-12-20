#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "trModule.h"
#include "trLog.h"

#include <list>

//#include <AK\SoundEngine\Common\AkTypes.h>

class AKEmitter;
class GameObject;

class trAudio : public trModule
{
public:

	trAudio();
	~trAudio();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	bool LoadSoundBank(const char* bank_path);
	bool UnloadSoundBank(const char* bank_path);

	AKEmitter* CreateEmitter(const char* name, GameObject* go);

private:
	bool InitWwise();

public:
	wchar_t* banks_directory = nullptr;

private:
	std::list<const char*> loaded_banks;
	std::list<AKEmitter*> firul_emitters;
};

#endif //__AUDIO_H__