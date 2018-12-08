#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "trModule.h"
#include "trLog.h"
#include "AK/SoundEngine/Common/AkSoundEngine.h"

class trAudio : public trModule
{
public:

	trAudio();
	~trAudio();

	bool Start();
	bool PreUpdate(float dt);
	bool Update(float dt);
	bool CleanUp();

private:

	bool InitWwise();
};

#endif //__AUDIO_H__