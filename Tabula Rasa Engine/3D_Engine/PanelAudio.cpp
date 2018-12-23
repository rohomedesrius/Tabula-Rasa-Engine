#include "PanelAudio.h"

#include "GameObject.h"
#include "trMainScene.h"
#include "trApp.h"
#include "trAudio.h"
#include "ComponentAudio.h"

#include "FirulWwise.h"

PanelAudio::PanelAudio() : Panel("Audio", SDL_SCANCODE_5)
{
	active = false;

}

PanelAudio::~PanelAudio()
{
}

void PanelAudio::Draw()
{
	ImGui::Begin("Audio", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing);

	GameObject* root = App->main_scene->GetRoot();
	
	int i = 0;
	
	if (root != nullptr)
	{
		std::list<GameObject*>::const_iterator it = root->childs.begin();
		while (it != root->childs.end())
		{
			go_names[i] = (*it)->GetName();
			it++;
			i++;
		}
	}
	
	if (ImGui::CollapsingHeader("Create Emitters"))
	{
		ImGui::Combo("Scene Gameobjects", &current_emitter, go_names, i);
		GameObject* temp = root->FindGOByName(go_names[current_emitter]);
		
		if (ImGui::Checkbox("Set as Emitter", &emitter))
			listener = !listener;

		ImGui::SameLine();

		if (ImGui::Checkbox("Set as Listener", &listener))
			emitter = !emitter;

		ImGui::Separator();

		char* type_names[] = { "SFX", "MUSIC" };
		static char event_name[41] = "";
		static char state_group[41] = "";
		static char state_a[41] = "";
		static char state_b[41] = "";

		static bool demo_music_event = true;
		static bool demo_spacial_event = true;

		static float transition = 8.0f;
		
		if (emitter)
		{
			ImGui::Combo("", &type_current, type_names, IM_ARRAYSIZE(type_names));
			ImGui::SameLine();
			ImGui::Text("Audio Type");

			switch (type_current)
			{
			case 0:

				if (ImGui::Checkbox("Demo Spacial Sound Event", &demo_spacial_event))
				{
					if (!demo_spacial_event)
					{
						sprintf_s(event_name, IM_ARRAYSIZE(event_name), "");
					}
				}
				if (demo_spacial_event)
				{
					sprintf_s(event_name, IM_ARRAYSIZE(event_name), "Play_Spatial_Sound");
				}

				ImGui::InputText("Event Name", event_name, 40);

				break;
			case 1:

				if (ImGui::Checkbox("Demo Music Event", &demo_music_event))
				{
					if (!demo_music_event)
					{
						sprintf_s(event_name, IM_ARRAYSIZE(event_name), "");
						sprintf_s(state_group, IM_ARRAYSIZE(state_group), "");
						sprintf_s(state_a, IM_ARRAYSIZE(state_a), "");
						sprintf_s(state_b, IM_ARRAYSIZE(state_b), "");
					}
				}
				if (demo_music_event)
				{
					sprintf_s(event_name, IM_ARRAYSIZE(event_name), "Play_Music");
					sprintf_s(state_group, IM_ARRAYSIZE(state_group), "Background");
					sprintf_s(state_a, IM_ARRAYSIZE(state_a), "SongA");
					sprintf_s(state_b, IM_ARRAYSIZE(state_b), "SongB");
				}
					ImGui::InputText("Event Name", event_name, 40);
					ImGui::Spacing();

					ImGui::Separator();
					ImGui::Text("Event Configuration");
					ImGui::SliderFloat("Duration", &transition, 3.0, 10.0, "%.f");
					ImGui::Spacing();
					ImGui::InputText("State group", state_group, 40);
					ImGui::InputText("State 1", state_a, 40);
					ImGui::InputText("State 2", state_b, 40);

				break;
			}
		}
		else if (listener)
		{

		}

		ImGui::Separator();;
		if (ImGui::Button("Set"))
		{
			if (emitter)
			{
				AKEmitter* temp_emit = App->audio->CreateEmitter(go_names[current_emitter], temp, false);

				ComponentAudio* temp_comp = AddComponentAudio(current_emitter);
				
				temp_comp->SetType((DEMONSTRATION_TYPE)type_current);
				temp_comp->CreateAudioEvent(event_name, transition, state_group, state_a, state_b);
				temp_comp->SetEmitter(temp_emit);
			}
			else if (listener)
			{
				AKEmitter* temp_emit = App->audio->CreateEmitter(go_names[current_emitter], temp, true);

				ComponentAudio* temp_comp = AddComponentAudio(current_emitter);

				temp_comp->SetEmitter(temp_emit);
			}
		}
		ImGui::SameLine();
		ImGui::Text("<= Press at the end!");
	}
	
	if (ImGui::CollapsingHeader("Volume"))
	{
		
	}

	if (ImGui::CollapsingHeader("Background music"))
	{

	}
	
	////SoundBanks =======================================================
	const char* banks_names[] = { "Music.bnk", "Spatial_Sounds.bnk" };
	
	if (ImGui::CollapsingHeader("SoundBanks", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Combo("", &bank_current, banks_names, IM_ARRAYSIZE(banks_names));
		
		if (ImGui::Button("Load"))
		{
			App->audio->LoadSoundBank(banks_names[bank_current]);
		}

		ImGui::SameLine();

		if (ImGui::Button("Unload"))
		{
			App->audio->UnloadSoundBank(banks_names[bank_current]);
		}
	}

	ImGui::End();
}

ComponentAudio * PanelAudio::AddComponentAudio(int current)
{
	GameObject* root = App->main_scene->GetRoot();

	GameObject* go = root->FindGOByName(go_names[current]);

	if (go != nullptr)
	{
		ComponentAudio* comp = (ComponentAudio*)go->CreateComponent(Component::component_type::COMPONENT_AUDIO);

		return comp;
	}

	return nullptr;
}