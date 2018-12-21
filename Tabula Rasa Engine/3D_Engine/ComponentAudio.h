#ifndef __COMPONENT_AUDIO_H__
#define __COMPONENT_AUDIO_H__

#include "FirulWwise.h"
#include "Component.h"

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

	bool Update(float dt);

	//Scene Management
	bool Save(JSON_Object* component_obj)const;
	bool Load(const JSON_Object* component_obj);

private:

	COMP_TYPE audio_comp_type = MUSIC;

};

#endif // __COMPONENT_AUDIO_H__