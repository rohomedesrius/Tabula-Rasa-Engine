#include "ComponentAudio.h"
#include "trApp.h"

ComponentAudio::ComponentAudio(GameObject* embedded_game_object) :
	Component(embedded_game_object, Component::component_type::COMPONENT_AUDIO)
{
}

ComponentAudio::~ComponentAudio()
{
}

bool ComponentAudio::Update(float dt)
{
	if (App->IsRunTime())
	{
		if (audio_comp_type == MUSIC)
		{
			emitter->PlayEvent("Play");
		}
	}
	
	return true;
}

void ComponentAudio::SetAudioCompType(COMP_TYPE type)
{
	audio_comp_type = type;
}

void ComponentAudio::CreateAudioEvent(const char * name)
{
	AudioEvent* new_event = new AudioEvent();
	new_event->name = name;

	audio_event = new_event;
}

bool ComponentAudio::Save(JSON_Object* component_obj)const
{
	return true;
}

bool ComponentAudio::Load(const JSON_Object* component_obj)
{
	return true;
}

void ComponentAudio::SetEmitter(AKEmitter* new_emitter)
{
	emitter = new_emitter;
}