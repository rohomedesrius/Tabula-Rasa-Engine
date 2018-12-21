#include "PanelAudio.h"

#include "GameObject.h"
#include "trMainScene.h"
#include "trApp.h"
#include "trAudio.h"

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
	
	if (ImGui::CollapsingHeader("Emitters"))
	{
		ImGui::Combo("Scene Gameobjects", &current_emitter, go_names, i);
	
		if (ImGui::Button("Set as Emitter"))
		{
			AddComponentAudio(current_emitter);
		}

		ImGui::SameLine();

		if (ImGui::Button("Set as Listener"))
		{
			AddComponentAudio(current_emitter);
		}

		//ImGui::ListBox();
	}
	
	if (ImGui::CollapsingHeader("Volume"))
	{
		
	}
	
	////SoundBanks =======================================================
	const char* banks_names[] = { "Main.bnk", "Music.bnk" };
	
	if (ImGui::CollapsingHeader("SoundBanks"))
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
	
	if (ImGui::CollapsingHeader("Test"))
	{
		if (ImGui::Button("Event 1"))
		{
			//AKEmitter::PlayEvent();
		}
	
		if (ImGui::Button("Event 2"))
		{
			//AKEmitter::PlayEvent();
		}
	
	}

	ImGui::End();
}

void PanelAudio::AddComponentAudio(int current)
{
	GameObject* root = App->main_scene->GetRoot();

	GameObject* go = root->FindGOByName(go_names[current]);

	if (go != nullptr)
	{
		go->CreateComponent(Component::component_type::COMPONENT_AUDIO);
	}
		
}