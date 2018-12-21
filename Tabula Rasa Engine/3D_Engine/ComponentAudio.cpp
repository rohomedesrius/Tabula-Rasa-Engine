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
		for (std::vector<AudioEvent*>::iterator it = posted_events.begin(); it != posted_events.end(); it++)
		{
			if ((*it)->rendering == false)
			{
				(*it)->rendering = true;
				emitter->PlayEvent((*it)->name.c_str());
			}
		}
	}
	
	return true;
}

void ComponentAudio::CreateAudioEvent(const char * name)
{
	AudioEvent* new_event = new AudioEvent();
	new_event->name = name;

	posted_events.push_back(new_event);
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