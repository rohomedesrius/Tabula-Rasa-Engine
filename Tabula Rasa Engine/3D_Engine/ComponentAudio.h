#ifndef __COMPONENT_AUDIO_H__
#define __COMPONENT_AUDIO_H__

#include "FirulWwise.h"
#include "Component.h"
#include "trPerfTimer.h"

#include <list>
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

	void CreateAudioEvent(const char* name, const float dura = 0.0f, const char * group = "", const char * a = "", const char * b = "");
	void SetEmitter(AKEmitter* new_emitter);

	void SetType(DEMONSTRATION_TYPE type);

	//Scene Management
	bool Save(JSON_Object* component_obj)const;
	bool Load(const JSON_Object* component_obj);
	void SaveAudioEvent();
	void LoadAudioEvent();

	//Position/Rotation Management
	void ManagePosRot();

private:
	
	DEMONSTRATION_TYPE demo_type = MUSIC;

	AudioEvent* audio_event;

	AKEmitter* emitter = nullptr;

	bool save = false;
	std::vector<std::string> to_save_str;
	float to_save_float = 0.0f;
};

#endif // __COMPONENT_AUDIO_H__