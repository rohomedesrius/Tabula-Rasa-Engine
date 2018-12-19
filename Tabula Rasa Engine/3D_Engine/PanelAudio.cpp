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
	static int go_current = 0;
	int i = 0;

	if (root != nullptr) {
		std::list<GameObject*>::const_iterator it = root->childs.begin();
		while (it != root->childs.end()) {
			go_names[i] = (*it)->GetName();
			it++;
			i++;
		}
	}

	if (ImGui::CollapsingHeader("Listeners"))
	{
		ImGui::Combo("Main Listener", &go_current, go_names, i);
	}

	if (ImGui::CollapsingHeader("Volume"))
	{
		ImGui::Combo("Main Listener", &go_current, go_names, i);
	}

	ImGui::End();
}