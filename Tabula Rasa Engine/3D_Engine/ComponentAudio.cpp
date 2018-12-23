#include "ComponentAudio.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "trApp.h"
#include "ImGui/imgui.h"

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
		static trPerfTimer timer;
		static bool new_cicle = false;

		for (std::vector<AudioEvent*>::iterator it = posted_events.begin(); it != posted_events.end(); it++)
		{
			if ((*it) != nullptr)
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
				else
				{

				}

				if (demo_type == MUSIC && new_cicle)
				{
					timer.Start();
					new_cicle = false;
				}
			}
		}
	}

	ManagePosRot();
	
	return true;
}

void ComponentAudio::CreateAudioEvent(const char * name, const float dura, const char* group, const char* a, const char* b)
{
	AudioEvent* new_event = new AudioEvent();
	new_event->name = name;
	new_event->transition = dura;
	new_event->state_group = group;
	new_event->state_a = a;
	new_event->state_b = b;

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

void ComponentAudio::SetType(DEMONSTRATION_TYPE type)
{
	demo_type = type;
}

void ComponentAudio::ManagePosRot()
{
	GameObject* temp_go = GetEmbeddedObject();

	if (temp_go != nullptr)
	{
		float3 current_pos = temp_go->GetTransform()->GetTranslation();
		Quat current_rot = temp_go->GetTransform()->GetRotation();

		float3 top = current_rot.Transform(float3(0, 1, 0));
		float3 front = current_rot.Transform(float3(0, 0, 1));

		emitter->SetPosition(current_pos.x, current_pos.y, current_pos.z, front.x, front.y, front.z, top.x, top.y, top.z);
	}
}
