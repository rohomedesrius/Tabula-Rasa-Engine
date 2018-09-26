#include "PanelConsole.h"
#include "imgui.h"

PanelConsole::PanelConsole(std::vector<std::string> init_logs) : Panel("Console", SDL_SCANCODE_1)
{
	active = false;

	std::vector<std::string>::iterator it = init_logs.begin();
	while (it != init_logs.end())
	{
		this->AddLogToConsole((*it).c_str());
		it++;
	}
}

PanelConsole::~PanelConsole()
{

}

void PanelConsole::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

	ImGui::Begin("Console", &active);
	ImGui::TextUnformatted(text_buffer.begin());

	if (have_to_scroll)
		ImGui::GetScrollMaxY();

	have_to_scroll = false;
	
	ImGui::End();
}

void PanelConsole::AddLogToConsole(const char * log)
{
	text_buffer.appendf(log);
	have_to_scroll = true;
}

void PanelConsole::ClearLog()
{
	text_buffer.clear();
}
