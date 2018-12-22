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
		was_playing = true;
		static trPerfTimer timer;
		static bool new_cicle = false;
		static bool paused = false;

		for (std::vector<AudioEvent*>::iterator it = posted_events.begin(); it != posted_events.end(); it++)
		{
			if (demo_type == SFX)
			{
				if ((*it)->rendering == false)
				{
					(*it)->rendering = true;
					emitter->PlayEvent((*it)->name.c_str());
				}
			}
			else if (demo_type == MUSIC)
			{

				if ((*it)->rendering == false)
				{
					timer.Start();

					(*it)->current_state = &(*it)->state_b;
					(*it)->rendering = true;

					emitter->PlayEvent((*it)->name.c_str());
					emitter->SetState((*it)->state_group.c_str(), (*it)->current_state->c_str());
				}
				else
				{
					if (timer.ReadMs() / 1000 > (*it)->transition)
					{
						new_cicle = true;

						if ((*it)->current_state == &(*it)->state_a)
							(*it)->current_state = &(*it)->state_b;
						else if ((*it)->current_state == &(*it)->state_b)
							(*it)->current_state = &(*it)->state_a;

						emitter->SetState((*it)->state_group.c_str(), (*it)->current_state->c_str());
					}
				}
			}
		}
		if (demo_type == MUSIC && new_cicle)
		{
			timer.Start();
			new_cicle = false;
		}

		if (App->IsPaused() && !paused)
		{
			paused = true;
			emitter->PauseAllEvent();
		}
		else if (paused)
		{
			if (!App->IsPaused())
			{
				paused = false;
				emitter->ResumeAllEvent();
			}
		}
	}
	else
	{
		if (was_playing)
		{
			for (std::vector<AudioEvent*>::iterator it = posted_events.begin(); it != posted_events.end(); it++)
			{
				emitter->StopEvent((*it)->name.c_str());
			}
			was_playing = false;
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