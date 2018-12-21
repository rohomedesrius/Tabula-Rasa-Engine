#ifndef __COMPONENT_AUDIO_H__
#define __COMPONENT_AUDIO_H__

#include "FirulWwise.h"
#include "Component.h"

#include <string>

enum COMP_TYPE
{
	MUSIC,
	LISTENER
};

class AudioEvent
{
public:

	std::string name = "";
	bool is_playing = false;

	std::string state_group = "";
	std::string state_a = "";
	std::string state_b = "";
	std::string* current_state = nullptr;

	float transition = 0.0f;
};

class ComponentAudio : public Component
{
public:
	ComponentAudio(GameObject* embedded_game_object);
	~ComponentAudio();

	bool Update(float dt);

	void SetAudioCompType(COMP_TYPE type);
	void CreateAudioEvent(const char* name);

	//Scene Management
	bool Save(JSON_Object* component_obj)const;
	bool Load(const JSON_Object* component_obj);

private:

	COMP_TYPE audio_comp_type = MUSIC;

	AudioEvent* audio_event = nullptr;
};

#endif // __COMPONENT_AUDIO_H__