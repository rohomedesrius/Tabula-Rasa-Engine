#ifndef __COMPONENT_AUDIO_H__
#define __COMPONENT_AUDIO_H__

#include "FirulWwise.h"
#include "Component.h"
#include "trPerfTimer.h"

#include <list>
#include <string>
#include <vector>

enum AUDIO_TYPE
{
	SFX = 0,
	MUSIC,
	NONE
};

class AudioEvent
{
public:

	std::string name = "";
	bool rendering = false;

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

	//Audio Management
	void CreateAudioEvent(const char* name, const float dura = 0.0f, const char * group = "", const char * a = "", const char * b = "");
	void CreateEmitter(bool is_listener, int type);
	void SetEmitter(AKEmitter* new_emitter);

	//Scene Management
	bool Save(JSON_Object* component_obj)const;
	bool Load(const JSON_Object* component_obj);

	//Position/Rotation Management
	void ManagePosRot();

	//Inspector UI
	void OnInspector();

private:

	AudioEvent* audio_event = nullptr;

	AKEmitter* emitter = nullptr;
};

#endif // __COMPONENT_AUDIO_H__