#include "ComponentAudio.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "trApp.h"
#include "trAudio.h"
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

		if (audio_event != nullptr)
		{
			if (emitter->GetType() == SFX)
			{
				if (audio_event->rendering == false)
				{
					audio_event->rendering = true;
					emitter->PlayEvent(audio_event->name.c_str());
				}
			}
			else if (emitter->GetType() == MUSIC)
			{
				if (audio_event->rendering == false)
				{
					timer.Start();

					audio_event->current_state = &audio_event->state_b;
					audio_event->rendering = true;

					emitter->PlayEvent(audio_event->name.c_str());
					emitter->SetState(audio_event->state_group.c_str(), audio_event->current_state->c_str());
				}
				else
				{
					if (timer.ReadMs() / 1000 > audio_event->transition)
					{
						new_cicle = true;

						if (audio_event->current_state == &audio_event->state_a)
							audio_event->current_state = &audio_event->state_b;
						else if (audio_event->current_state == &audio_event->state_b)
							audio_event->current_state = &audio_event->state_a;

						emitter->SetState(audio_event->state_group.c_str(), audio_event->current_state->c_str());
					}
				}
			}
			else
			{

			}

			if (emitter->GetType() == MUSIC && new_cicle)
			{
				timer.Start();
				new_cicle = false;
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

	audio_event = new_event;
}

void ComponentAudio::CreateEmitter(bool is_listener, int type)
{
	emitter = App->audio->CreateEmitter(this->GetEmbeddedObject()->GetName(), this->GetEmbeddedObject(), is_listener, type);
}

bool ComponentAudio::Save(JSON_Object* component_obj)const
{
	// Emitter
	json_object_set_number(component_obj, "Em_Type", emitter->GetType());
	json_object_set_boolean(component_obj, "Em_Listener", emitter->IsListener());

	// Event
	if (emitter->GetType() != NONE)
	{
		json_object_set_string(component_obj, "E_Name", audio_event->name.c_str());
		json_object_set_string(component_obj, "E_Group", audio_event->state_group.c_str());
		json_object_set_string(component_obj, "E_A", audio_event->state_a.c_str());
		json_object_set_string(component_obj, "E_B", audio_event->state_b.c_str());
		json_object_set_number(component_obj, "E_Transition", audio_event->transition);
	}
	return true;
}

bool ComponentAudio::Load(const JSON_Object* component_obj)
{
	// Emitter
	JSON_Value* value = json_object_get_value(component_obj, "Em_Listener");
	JSON_Value* value_2 = json_object_get_value(component_obj, "Em_Type");
	CreateEmitter(json_value_get_string(value), json_value_get_number(value_2));

	// Event
	if (json_value_get_number(value_2) != NONE)
	{
		value = json_object_get_value(component_obj, "E_Name");
		CreateAudioEvent(json_value_get_string(value));

		value = json_object_get_value(component_obj, "E_Group");
		audio_event->state_group = json_value_get_string(value);
		value = json_object_get_value(component_obj, "E_A");
		audio_event->state_a = json_value_get_string(value);
		value = json_object_get_value(component_obj, "E_B");
		audio_event->state_b = json_value_get_string(value);
		value = json_object_get_value(component_obj, "E_Transition");
		audio_event->transition = json_value_get_number(value);
	}

	return true;
}

void ComponentAudio::SetEmitter(AKEmitter* new_emitter)
{
	emitter = new_emitter;
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

void ComponentAudio::OnInspector()
{
	if (emitter != nullptr)
	{
		std::string str;
		switch (emitter->GetType())
		{
		case 0:
			str = "EMITTER [SFX]"; break;
		case 1:
			str = "EMITTER [MUSIC]"; break;
		case 2:
			str = "LISTENER"; break;
		}

		ImGui::Text("Type: %s", str);

		if (emitter->GetType() == MUSIC)
		{
			ImGui::SliderFloat("Duration##dur", &audio_event->transition, 4.0f, 15.0f, "%.fs");
		}
	}
}
