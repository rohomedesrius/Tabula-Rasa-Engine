#include "PanelControl.h"
#include "trApp.h"
#include "trWindow.h"
#include "trTimeManager.h"

PanelControl::PanelControl() : Panel("Control")
{
	active = true;
}

PanelControl::~PanelControl()
{

}

void PanelControl::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, 16.0f), ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(ImVec2(App->window->GetWidth(), 25.0f), ImGuiSetCond_Always);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
	ImGui::Begin("", &active,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar);

	ImGui::PopStyleColor();

	ImVec2 new_btn_pos(ImGui::GetWindowPos().x + (App->window->GetWidth() / 2.0f - 100.0f), ImGui::GetCursorScreenPos().y);
	ImGui::SetCursorScreenPos(new_btn_pos);
	static std::string play_btn = "PLAY";

	if (ImGui::Button(play_btn.c_str()))
	{
		App->SwitchRunTime();

		if (App->IsRunTime())
		{
			play_btn = "STOP";
		}
		else {
			play_btn = "PLAY";
			App->time_manager->ReStartGameClock();
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("PAUSE"))
	{
		App->time_manager->PauseGameClock();
		App->SetPaused();
	}

	ImGui::SameLine();

	if (ImGui::Button("ADVANCE"))
		App->time_manager->StepGameClock();

	ImGui::End();
}