#include <iostream> 

#include "trApp.h"
#include "trDefs.h"
#include "trLog.h"
#include "trWindow.h"
#include "trInput.h"
#include "trRenderer3D.h"
#include "trCamera3D.h"
#include "trEditor.h"
#include "trHardware.h"
#include "trFileLoader.h"
#include "trTimeManager.h"
#include "trFileSystem.h"
#include "trResources.h"
#include "trAudio.h"

#include "trMainScene.h"

#include <algorithm>
#include "pcg/pcg_basic.h"


// Constructor
trApp::trApp(int argc, char* args[]) : argc(argc), args(args)
{
	pcg32_random();

	frames = 0;
	last_frame_ms = -1;
	last_fps = -1;
	capped_ms = 1000 / 60;
	fps_counter = 0;

	//randomizator = new math::LCG();

	input = new trInput();
	window = new trWindow();
	render = new trRenderer3D();
	camera = new trCamera3D();
	main_scene = new trMainScene();
	hardware = new trHardware();
	file_loader = new trFileLoader();
	editor = new trEditor();
	time_manager = new trTimeManager();
	file_system = new trFileSystem();
	resources = new trResources();
	audio = new trAudio();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp

	AddModule(input);
	AddModule(window);
	AddModule(camera);
	AddModule(editor);
	AddModule(main_scene);
	AddModule(hardware);
	AddModule(file_loader);
	AddModule(time_manager);
	AddModule(file_system);
	AddModule(resources);
	AddModule(audio);
	

	// render last to swap buffer
	AddModule(render);

	
	//disable modules here
}

// Destructor
trApp::~trApp()
{
	// release modules
	
	for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		RELEASE(*it);
	}

	modules.clear();
}

void trApp::AddModule(trModule* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool trApp::Awake()
{
	bool ret = true;

	JSON_Value* root_value = nullptr;
	root_value = json_parse_file("Settings/settings.json");

	if (root_value != nullptr) {

		JSON_Object* root_obj = json_value_get_object(root_value);

		TR_LOG("trApp: config.json loaded correctly, iterating between modules ...");
		JSON_Object* app_obj = json_object_get_object(json_value_get_object(root_value), "app");
		this->SetTitle(json_object_get_string(app_obj, "title"));
		this->SetOrganization(json_object_get_string(app_obj, "organization"));
		this->SetFpsCap(json_object_get_number(app_obj, "framerate_cap"));
		cap_fps = json_object_get_boolean(app_obj, "cap_framerate");
		this->SetVersion(json_object_get_string(app_obj, "version"));

		for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
		{
			JSON_Object* module_obj = json_object_get_object(root_obj, (*it)->name.c_str());

			ret = (*it)->Awake(module_obj);
			if(!ret)
				TR_LOG("trApp: Error awakening in: %s", (*it)->name.c_str());
		}

		json_value_free(root_value);
	}
	else {

		TR_LOG("trApp: Error loading config.json file");

		this->SetTitle(A_TITLE);
		this->SetOrganization(A_ORGANIZATION);
		this->SetFpsCap(A_FPS_CAP_VALUE);
		cap_fps = A_FPS_CAP;
		this->SetVersion(A_VERSION);

		for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
			ret = (*it)->Awake();

	}
		

	return ret;
}

// Called before the first frame
bool trApp::Start()
{
	bool ret = true;

	std::list<trModule*>::iterator it = modules.begin();
	
	while (it != modules.end() && ret == true)
	{
		if ((*it)->active == false) {
			it++;
			continue;
		}
		ret = (*it)->Start();
		it++;
	}

	return ret;
}

// Called each loop iteration
bool trApp::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (ret)
		ret = PreUpdate();

	if (ret)
		ret = DoUpdate();

	if (ret)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
void trApp::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.f;

	if(run_time)
		time_manager->UpdateGameClock(dt);
	time_manager->UpdateRealTimeClock(dt);

	ms_timer.Start();
}

// ---------------------------------------------
void trApp::FinishUpdate()
{
	// Framerate calculations --

	if (want_to_save == true) {
		SaveNow();
		want_to_save = false;
	}

	if (want_to_load == true) {
		LoadNow();
		want_to_load = false;
	}

	++frames;
	++fps_counter;

	if (fps_timer.Read() > 1000.f)
	{
		last_fps = fps_counter;
		fps_counter = 0;
		fps_timer.Start();
	}

	last_frame_ms = ms_timer.Read();

	editor->InfoFPSMS((float)last_fps, (float)ms_timer.Read(), frames);

	// Cap fps
	if (cap_fps && capped_ms > 0 && (last_frame_ms < capped_ms))
		SDL_Delay(capped_ms - last_frame_ms);

	if (!all_modules_loaded)
		all_modules_loaded = true;
}

// Call modules before each loop iteration
bool trApp::PreUpdate()
{
	bool ret = true;

	trModule* pModule = NULL;

	for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
	{
		pModule = (*it);

		if (pModule->active == false) {
			continue;
		}

		if (run_time) {
			if (pModule == main_scene) {
				ret = (*it)->PreUpdate(time_manager->GetGameDt());
			}
			else {
				ret = (*it)->PreUpdate(dt);
			}
		}else
			ret = (*it)->PreUpdate(dt);
		
	}

	return ret;
}

// Call modules on each loop iteration
bool trApp::DoUpdate()
{

	bool ret = true;

	trModule* pModule = NULL;

	for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
	{
		pModule = (*it);

		if (pModule->active == false) {
			continue;
		}

		if (all_modules_loaded) {
			if (run_time) {
				if (pModule == main_scene) {
					ret = (*it)->Update(time_manager->GetGameDt());
				}
				else {
					ret = (*it)->Update(dt);
				}
			}
			else
				ret = (*it)->Update(dt);
		}
		else {
			ret = (*it)->Update(0);
		}
	}

	return ret;
}

// Call modules after each loop iteration
bool trApp::PostUpdate()
{
	bool ret = true;

	trModule* pModule = NULL;

	for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
	{
		pModule = (*it);

		if (pModule->active == false) {
			continue;
		}

		if (run_time) {
			if (pModule == main_scene) {
				ret = (*it)->PostUpdate(time_manager->GetGameDt());
			}
			else {
				ret = (*it)->PostUpdate(dt);
			}
		}
		else
			ret = (*it)->PostUpdate(dt);
	}

	//PERF_PEEK(ptimer);
	return ret;
}

// Called before quitting
bool trApp::CleanUp()
{
	bool ret = true;
	
	std::list<trModule*>::reverse_iterator it = modules.rbegin();

	while (it != modules.rend() && ret == true)
	{
		ret = (*it)->CleanUp();
		it++;
	}

	return ret;
}

// ---------------------------------------
int trApp::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* trApp::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* trApp::GetTitle() const
{
	return game_title.data();
}

void trApp::SetTitle(const char * title)
{
	game_title = title;
}

// ---------------------------------------
const char* trApp::GetOrganization() const
{
	return organization.data();
}

void trApp::RequestBrowser(const char * url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void trApp::SendTheCarrierPigeon(const Event & event)
{
	for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end(); it++)
		(*it)->OnEventReceived(event);
}

void trApp::SetOrganization(const char * organization)
{
	this->organization = organization;
}

void trApp::SetVersion(const char * version)
{
	this->version = version;
}


uint trApp::GetFpsCap() const
{
	if (capped_ms > 0)
		return (uint)((1.0f / (float)capped_ms) * 1000.0f);
	else
		return 0;
}

const char * trApp::GetVersion() const
{
	return version.c_str();
}

UID trApp::GenerateNewUUID()
{
	return pcg32_boundedrand_r(&pcg32_global, UINT32_MAX);
}

void trApp::SetFpsCap(uint max_framerate)
{
	if (max_framerate > 0)
		capped_ms = 1000 / max_framerate;
	else
		capped_ms = 0;
}

void trApp::SetPaused()
{
	paused = !paused;
}

void trApp::SwitchRunTime()
{
	run_time = !run_time;
	if (run_time) {
		App->main_scene->SerializeScene(temporal_scene, "Temporal_Scene");
	}
	else {
		// Clear
		App->main_scene->ClearScene(true);

		char* buffer = nullptr;
		uint size = App->file_system->ReadFromFile(temporal_scene.c_str(), &buffer);

		if (buffer != nullptr && size > 0)
		{
			App->main_scene->DeSerializeScene(buffer);
			RELEASE_ARRAY(buffer);
		}
		else
			TR_LOG("trApp: Error reading scene from path: %s", temporal_scene.c_str());
	}
}

bool trApp::IsRunTime() const
{
	return run_time;
}

bool trApp::IsPaused() const
{
	return paused;
}

void trApp::Save()
{
	want_to_save = true;
}

void trApp::Load()
{
	want_to_load = true;
}


bool trApp::LoadNow()
{
	TR_LOG("Loading ...");

	bool ret = true;

	JSON_Value* root_value = nullptr;
	root_value = json_parse_file("Settings/config.json");

	if (root_value != nullptr) {

		TR_LOG("settings.json loaded correctly, iterating between modules ...");

		for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
		{
			trModule* pModule = (*it);

			if (pModule->active == false) {
				continue;
			}

			JSON_Object* module_obj = json_object_get_object(json_value_get_object(root_value), (*it)->name.c_str());

			ret = (*it)->Load(module_obj);
		}
		json_value_free(root_value);
	}
	else {
		TR_LOG("trApp: Error loading settings.json file");
		for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
		{
			trModule* pModule = (*it);

			if (pModule->active == false) {
				continue;
			}
			ret = (*it)->Load();
		}
	}
		
	
	
	return ret;
}

bool trApp::SaveNow()
{

	TR_LOG("Saving ...");

	bool ret = true;

	JSON_Value* root_value = json_value_init_object();
	JSON_Object* root_obj = json_value_get_object(root_value);

	// Application settings
	JSON_Value* app_value = json_value_init_object();
	JSON_Object* app_obj = json_value_get_object(app_value);
	json_object_set_value(root_obj, "app", app_value);
	json_object_set_string(app_obj, "title", App->GetTitle());
	json_object_set_string(app_obj, "organization", App->GetOrganization());
	json_object_set_string(app_obj, "version", App->GetVersion());
	json_object_set_number(app_obj, "framerate_cap", capped_ms);
	json_object_set_boolean(app_obj, "cap_framerate", cap_fps);
	
	char *serialized_string = NULL;

	for (std::list<trModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
	{
		trModule* pModule = (*it);

		if (pModule->active == false) {
			continue;
		}
		JSON_Value* mod_value = json_value_init_object();
		JSON_Object* mod_obj = json_value_get_object(mod_value);
		json_object_set_value(root_obj, (*it)->name.c_str(), mod_value);

		ret = (*it)->Save(mod_obj);
	}
	
	serialized_string = json_serialize_to_string_pretty(root_value);
	puts(serialized_string);
	json_serialize_to_file(root_value, "Settings/settings.json");
	json_free_serialized_string(serialized_string);
	json_value_free(root_value);

	return ret;
}