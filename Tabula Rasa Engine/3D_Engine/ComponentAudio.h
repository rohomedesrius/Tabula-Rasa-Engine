#ifndef __COMPONENT_AUDIO_H__
#define __COMPONENT_AUDIO_H__

#include "Component.h"
#include "FirulWwise.h"

enum COMP_TYPE
{
	MUSIC,
	LISTENER
};

class ComponentAudio : public Component
{
public:
	ComponentAudio(GameObject* embedded_game_object);
	~ComponentAudio();

private:

};

#endif // __COMPONENT_AUDIO_H__