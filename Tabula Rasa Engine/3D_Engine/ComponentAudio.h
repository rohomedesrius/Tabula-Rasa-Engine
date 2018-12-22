#ifndef __COMPONENT_AUDIO_H__
#define __COMPONENT_AUDIO_H__

#include "FirulWwise.h"
#include "Component.h"
#include "trPerfTimer.h"

#include <string>
#include <vector>

enum DEMONSTRATION_TYPE
{
	SFX,
	MUSIC,
	NONE
};

class AudioEvent
{
public:

	std::string name = "Play";
	bool rendering = false;

	// TMP hardcoded
	std::string state_group = "Background";
	std::string state_a = "SongA";
	std::string state_b = "SongB";
	std::string* current_state = &state_a;

	float transition = 0.0f;
};

class ComponentAudio : public Component
{
public:
	ComponentAudio(GameObject* embedded_game_object);
	~ComponentAudio();

	bool Update(float dt);

	void CreateAudioEvent(const char* name);
	void SetEmitter(AKEmitter* new_emitter);

	//Scene Management
	bool Save(JSON_Object* component_obj)const;
	bool Load(const JSON_Object* component_obj);

private:
	
	DEMONSTRATION_TYPE demo_type = NONE;

	std::vector<AudioEvent*> posted_events;

	AKEmitter* emitter = nullptr;

	bool was_playing = false;
};

#endif // __COMPONENT_AUDIO_H__