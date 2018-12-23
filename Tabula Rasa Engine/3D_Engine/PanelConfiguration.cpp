#include "PanelConfiguration.h"
#include "trApp.h"
#include "trModule.h"
#include "trHardware.h"
#include "trWindow.h"
#include "trInput.h"
#include "trRenderer3D.h"
#include "trCamera3D.h"
#include "ComponentCamera.h"
#include "trEditor.h"
#include "trTimeManager.h"
#include "mmgr/mmgr.h"
#include "trTimer.h"

#include "Glew/include/GL/glew.h"

using namespace std;

PanelConfiguration::PanelConfiguration() : Panel("Configuration", SDL_SCANCODE_2),
chart_fps(CHART_FPS_CAP), chart_ms(CHART_FPS_CAP)
{
	active = false;
	time_scale = 1.0f;
	mem_timer.Start();
}

PanelConfiguration::~PanelConfiguration()
{}

void PanelConfiguration::Draw()
{
	ImGui::Begin("Configuration", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing);

	ShowApplication();

	if (SetUpCollapsingHeader(App->window))
		ShowWindow(App->window);

	if (SetUpCollapsingHeader(App->render))
		ShowRenderer(App->render);

	if (SetUpCollapsingHeader(App->input))
		ShowInput(App->input);

	if (SetUpCollapsingHeader(App->hardware))
		ShowHardware(App->hardware);

	if(SetUpCollapsingHeader(App->camera))
		ShowCamera(App->camera);

	ShowEngineClocks();

	ImGui::End();
}

bool PanelConfiguration::SetUpCollapsingHeader(trModule* module)
{
	bool ret = false;

	if (ImGui::CollapsingHeader(module->name.c_str()))
	{
		(module->active) ? ImGui::Text("Module Actived") : ImGui::Text("Module Disabled");
		ImGui::SameLine();

		if (module->name != "Renderer3D") { /// Renderer cant be disabled
			bool is_active = module->active;
			if (ImGui::Checkbox("Active", &is_active)) {
				module->TurnActive();
				module->CleanUp();
			}
		}

		ret = true;
	}

	return ret;
}

void PanelConfiguration::ShowApplication()
{
	if (ImGui::CollapsingHeader("Application"))
	{
		strcpy_s(app_name, TITLE_MAX_LENGTH, App->GetTitle());
		if (ImGui::InputText("App Name", app_name, TITLE_MAX_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->window->SetTitle(app_name);

		strcpy_s(org_name, TITLE_MAX_LENGTH, App->GetOrganization());
		if (ImGui::InputText("Organization", org_name, TITLE_MAX_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->SetOrganization(org_name);

		int fps_cap = App->GetFpsCap();
		if (ImGui::SliderInt("Max FPS", &fps_cap, 0, 120))
			App->SetFpsCap(60);

		ImGui::Text("Current FPS cap:");
		ImGui::SameLine();
		
		ImGui::TextColored(IMGUI_YELLOW, "%i", App->GetFpsCap());

		sprintf_s(title, 25, "Framerate %.1f", chart_fps.back());
		ImGui::PlotHistogram("##framerate", &chart_fps.front(), chart_fps.size(), 0, title, 0.0f, 100.0f, ImVec2(400, 90));
		sprintf_s(title, 25, "Milliseconds %0.1f", chart_ms.back());
		ImGui::PlotHistogram("##milliseconds", &chart_ms.front(), chart_ms.size(), 0, title, 0.0f, 40.0f, ImVec2(400, 90));

		ImGui::Separator();
		
		CalculateReportedMemory();
	}
}

void PanelConfiguration::CalculateReportedMemory()
{
	sMStats mem_stats = m_getMemoryStatistics();
	float total_mem = 0.f;

	if (mem_timer.Read() > 16.0f)
	{
		mem_timer.Start();
		if (first_mem_pass)
		{
			for (uint i = 0; i < CHART_MEM - 1; i++)
				mem_list[i] = mem_list[i + 1];

			total_mem = (float)mem_stats.totalReportedMemory;
			mem_list.back() = total_mem;
		}
		else
		{
			if (mem_list.size() == CHART_MEM)
				first_mem_pass = true;
			else
			{
				total_mem = (float)mem_stats.totalReportedMemory;
				mem_list.push_back(total_mem);
			}
		}
	}

	ImGui::PlotHistogram("##memory", &mem_list.front(), mem_list.size(), 0, "Memory Consumption", 0.0f, (float)mem_stats.peakReportedMemory * 1.2f, ImVec2(310, 100));

	ImGui::Text("Total Reported Mem: %u", mem_stats.totalReportedMemory);
	ImGui::Text("Total Actual Mem: %u", mem_stats.totalActualMemory);
	ImGui::Text("Peak Reported Mem: %u", mem_stats.peakReportedMemory);
	ImGui::Text("Peak Actual Mem: %u", mem_stats.peakActualMemory);
	ImGui::Text("Accumulated Reported Mem: %u", mem_stats.accumulatedReportedMemory);
	ImGui::Text("Accumulated Actual Mem: %u", mem_stats.accumulatedActualMemory);
	ImGui::Text("Accumulated Alloc Unit Count: %u", mem_stats.accumulatedAllocUnitCount);
	ImGui::Text("Total Alloc Unit Count: %u", mem_stats.totalAllocUnitCount);
	ImGui::Text("Peak Alloc Unit Count: %u", mem_stats.peakAllocUnitCount);
}

void PanelConfiguration::ShowHardware(trHardware * module)
{
	trHardware::HWInfo info = module->GetHardwareInfo();
	App->hardware->UpdateVRAMInfo();

	std::string info_str;
	info_str = "v%u.%u.%u";
	ImGui::Text("SDL Version:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, info_str.c_str(), info.sdl_version[0], info.sdl_version[1], info.sdl_version[2]);

	ImGui::Separator();
	info_str = "%u (Cache: %ukb)";
	ImGui::Text("CPUs:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, info_str.c_str(), info.cpu_count, info.cache_line_size);

	info_str = "%.1fGb";
	ImGui::Text("System RAM:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, info_str.c_str(), info.system_ram);

	info_str = "%s%s%s%s%s%s";
	ImGui::Text("Caps:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, info_str.c_str(), info.has_rtdcs ? "RDTSC," : "",
		info.has_alti_vec ? "AltiVec," : "",
		info.has_mmx ? "MMX," : "",
		info.has_3d_now ? "3DNow," : "",
		info.has_sse ? "SSE," : "",
		info.has_sse2 ? "SSE2," : "");

	info_str = "%s%s%s%s%s";
	ImGui::Text("");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, info_str.c_str(), info.has_sse3 ? "SSE3," : "",
		info.has_sse41 ? "SSE41," : "",
		info.has_sse42 ? "SSE42," : "",
		info.has_avx ? "AVX," : "",
		info.has_avx2 ? "AVX2" : "");

	ImGui::Separator();

	ImGui::Text("GPU: ");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%s", (char*)glGetString(GL_VENDOR));

	ImGui::Text("Brand: ");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%s", (char*)glGetString(GL_RENDERER));

	float updated_vram_budget = CONV_MEM_UP((float)info.vram_budget);
	float updated_vram_usage = CONV_MEM_UP((float)info.vram_usage);
	float updated_vram_available = CONV_MEM_UP((float)info.vram_available);
	float updated_vram_reserved = CONV_MEM_UP((float)info.vram_reserved);

	ImGui::Text("VRAM Budget: ");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%.2f%s", updated_vram_budget, " Mb");

	ImGui::Text("VRAM Usage: ");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%.2f%s", updated_vram_usage, " Mb");

	ImGui::Text("VRAM Available: ");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%.2f%s", updated_vram_available, " Mb");

	ImGui::Text("VRAM Reserved: ");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%.2f%s", updated_vram_reserved, " Mb");

	ImGui::Separator();

	info_str = "%s";
	ImGui::Text("OpenGL version supported: ");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, info_str.c_str(), (char*)glGetString(GL_VERSION));

	info_str = "%s";
	ImGui::Text("GLSL: ");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, info_str.c_str(), (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

}

void PanelConfiguration::ShowInput(trInput * module)
{
	ImGui::Text("Mouse Position:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%i,%i", module->GetMouseX(), module->GetMouseY());
	
	ImGui::Text("Mouse Motion:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%i,%i", module->GetMouseXMotion(), module->GetMouseYMotion());
	
	ImGui::Text("Mouse Wheel:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%i", module->GetMouseZ());
}

void PanelConfiguration::ShowWindow(trWindow * module)
{
	float brightness = App->window->GetBrightness();
	if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
		App->window->SetBrightness(brightness);

	uint width = App->window->GetWidth();
	uint height = App->window->GetHeight();

	if (ImGui::SliderInt("Width", (int*)&width, MIN_WIDTH, MAX_WIDTH))
		App->window->SetWidth(width);

	if (ImGui::SliderInt("Height", (int*)&height, MIN_HEIGHT, MAX_HEIGHT))
		App->window->SetHeight(height);
	
	ImGui::Text("Refresh rate:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%u", App->window->GetMonitorRefreshRate());

	// TODO: have to run again the engine!
	if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen))
		App->window->SetFullscreen(App->window->fullscreen);

	ImGui::SameLine();
	if (ImGui::Checkbox("Resizable", &App->window->resizable))
		App->window->SetResizable(App->window->resizable);

	if (ImGui::Checkbox("Borderless", &App->window->borderless))
		App->window->SetBorderless(App->window->borderless);

	ImGui::SameLine();
	if (ImGui::Checkbox("Full Desktop", &App->window->fullscreen_desktop))
		App->window->SetFullscreenWindowed(App->window->fullscreen_desktop);
}

void PanelConfiguration::ShowRenderer(trRenderer3D * module)
{
	ImGui::Separator();

	ImGui::Text("Debug draw");
	ImGui::SameLine();
	if (ImGui::Checkbox("##DEBUGDRAW", &App->render->debug_draw_on));
		App->render->SwitchDebugDraw(App->render->debug_draw_on);

	ImGui::Separator();

	ImGui::Text("Vsync");
	ImGui::SameLine();
	if (ImGui::Checkbox("##VSYNC", &App->render->vsync_toogle))
		App->render->SwitchVsync(App->render->vsync_toogle);

	ImGui::Separator();

	ImGui::Text("Switch wireframe");
	ImGui::SameLine();
	if (ImGui::Checkbox("##WIREFRAME", &App->render->wireframe))
		App->render->SwitchWireframeMode(App->render->wireframe);

	ImGui::Separator();

	ImGui::Text("Switch Depth Test");
	ImGui::SameLine();
	if (ImGui::Checkbox("##DEPTH", &App->render->depth_test))
		App->render->SwitchDepthMode(App->render->depth_test);

	ImGui::Separator();

	ImGui::Text("Switch Z-Buffer Visualization");
	ImGui::SameLine();
	ImGui::Checkbox("##ZBUFFER", &App->render->z_buffer);

	ImGui::Separator();

	ImGui::Text("Switch Cull Face");
	ImGui::SameLine();
	if (ImGui::Checkbox("##CULL_FACE", &App->render->cull_face))
		App->render->SwitchFaceCulling(App->render->cull_face);

	ImGui::Separator();

	ImGui::Text("Switch Lighting");
	ImGui::SameLine();
	if (ImGui::Checkbox("##LIGHTING", &App->render->lighting))
		App->render->SwitchLighting(App->render->lighting);

	ImGui::Separator();

	ImGui::Text("Switch Color Material");
	ImGui::SameLine();
	if (ImGui::Checkbox("##COLOR_MATERIAL", &App->render->color_material))
		App->render->SwitchColorMaterial(App->render->color_material);

	ImGui::Separator();

	ImGui::Text("Switch Texture 2D");
	ImGui::SameLine();
	if (ImGui::Checkbox("##TEXTURE2D", &App->render->texture_2D))
		App->render->SwitchTexture2D(App->render->texture_2D);
}

void PanelConfiguration::ShowCamera(trCamera3D * module)
{
	ImGui::Separator();
	ImGui::Text("Clipping Planes");

	if (ImGui::SliderFloat("Near plane##nearplane", &App->camera->dummy_camera->frustum.nearPlaneDistance, 0.5f, 995.0f));
		App->camera->dummy_camera->projection_needs_update = true;

	if (ImGui::SliderFloat("Far plane##farplane", &App->camera->dummy_camera->frustum.farPlaneDistance, 0.5f, 1000.0f))
		App->camera->dummy_camera->projection_needs_update = true;

	ImGui::Separator();

	float fov = math::RadToDeg(App->camera->dummy_camera->frustum.verticalFov);

	if (ImGui::SliderFloat("Field of View", &fov, 1.0f, 179.0f))
		App->camera->dummy_camera->SetUpFov(fov);
}

void PanelConfiguration::ShowEngineClocks()
{
	if (ImGui::CollapsingHeader("Engine clocks"))
	{
		ImGui::Text("Real time clock: %.2f sec", App->time_manager->ReadRealTimeClockSec());
		ImGui::Text("Real time clock (hour format): %02d : %02d : %02d",
			App->time_manager->ReadRealTimeHourFormat().hours,
			App->time_manager->ReadRealTimeHourFormat().min,
			App->time_manager->ReadRealTimeHourFormat().sec);

		ImGui::Separator();

		ImGui::Text("Game clock: %.2f sec", App->time_manager->ReadGameClockSec());		
		ImGui::Text("Game clock (hour format): %02d : %02d : %02d", 
				   App->time_manager->ReadGameHourFormat().hours, 
				   App->time_manager->ReadGameHourFormat().min,
				   App->time_manager->ReadGameHourFormat().sec);

		if (ImGui::Button("PLAY")) {
			if (App->IsRunTime()) {

			}
			else {
				App->time_manager->ReStartGameClock();
			}
			App->SwitchRunTime();
		}
			

		ImGui::SameLine();

		if (ImGui::Button("PAUSE"))
			App->time_manager->PauseGameClock();

		ImGui::SameLine();

		if (ImGui::Button("ADVANCE"))
			App->time_manager->StepGameClock();

		ImGui::SliderFloat("Time scale", &time_scale, 0.0f, 3.0f, "%.2f");
		App->time_manager->SetGameClockTimeScale(time_scale);
	}
}

void PanelConfiguration::FillChartFpsInfo(float fps, float ms, int frames)
{
	if (frames % (CHART_FPS_CAP + 1) == 0 || frames > CHART_FPS_CAP + 1)
	{
		for (uint i = 0; i < CHART_FPS_CAP - 1; i++)
		{
			chart_ms[i] = chart_ms[i + 1];
			chart_fps[i] = chart_fps[i + 1];
		}
	}
	else
		index++;
	
	chart_ms[index - 1] = ms;
	chart_fps[index - 1] = fps;
}