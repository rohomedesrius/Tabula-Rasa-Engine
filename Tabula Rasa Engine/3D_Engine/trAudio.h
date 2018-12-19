#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "trModule.h"
#include "trLog.h"

//#include <AK\SoundEngine\Common\AkTypes.h>

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

public:
	wchar_t* banks_directory = nullptr;

private:

	bool InitWwise();
};

#endif //__AUDIO_H__