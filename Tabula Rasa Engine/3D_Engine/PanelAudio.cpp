#include "PanelAudio.h"

#include "GameObject.h"
#include "trMainScene.h"
#include "trApp.h"

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

	if (root != nullptr) {
		std::list<GameObject*>::const_iterator it = root->childs.begin();
		while (it != root->childs.end()) {
			go_names.push_back((*it)->GetName());
			it++;
		}
	}

	if (ImGui::CollapsingHeader("Listeners"))
	{
		//ImGui::ListBox("Main Listener", 0, go_names.c_str());
	}

	if (ImGui::CollapsingHeader("Volume"))
	{

	}

	ImGui::End();
}