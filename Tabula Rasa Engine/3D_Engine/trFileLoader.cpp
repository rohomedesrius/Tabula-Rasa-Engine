#include "trFileLoader.h"

#include "trDefs.h"
#include "trApp.h"
#include "trFileSystem.h"

#include "trMainScene.h"

#include "MeshImporter.h"
#include "MaterialImporter.h"

trFileLoader::trFileLoader()
{
	this->name = "FileLoader";
}

trFileLoader::~trFileLoader()
{}

bool trFileLoader::Awake(JSON_Object* config)
{
	mesh_importer = new MeshImporter();
	material_importer = new MaterialImporter();
	return true;
}

bool trFileLoader::Start()
{
	// Create basic folders
	App->file_system->MakeNewDir(ASSETS_DIR);
	App->file_system->MakeNewDir(A_MODELS_DIR);
	App->file_system->MakeNewDir(A_SCENES_DIR);
	App->file_system->MakeNewDir(A_TEXTURES_DIR);

	App->file_system->MakeNewDir(SETTINGS_DIR);

	App->file_system->MakeNewDir(LIBRARY_DIR);
	App->file_system->MakeNewDir(L_MESHES_DIR);
	App->file_system->MakeNewDir(L_MATERIALS_DIR);

	return true;
}

// Called before quitting or switching levels
bool trFileLoader::CleanUp()
{
	TR_LOG("Cleaning File Loader");

	RELEASE(mesh_importer);
	RELEASE(material_importer);

	return true;
}

void trFileLoader::ImportFBX(const char* file_path)
{
	TR_LOG("trFileLoader: Start importing a file with path: %s", file_path);

	// TODO: copy paste FBX file to Assets/

	// Then get the file_path from de Assets continue

	std::string output;
	mesh_importer->Import(file_path, output);

}

void trFileLoader::ImportTexture(const char * file_path)
{
	// TODO: copy paste FBX file to Assets/

	// Then get the file_path from de Assets continue
}

void trFileLoader::ImportScene(const char * file_path)
{
	// TODO: copy paste FBX file to Assets/

	// Then get the file_path from de Assets continue

	if (file_path != nullptr)
	{
		std::string tmp_str(A_SCENES_DIR);
		tmp_str.append("/");
		tmp_str.append(file_path);

		// Clear previous scene
		App->main_scene->ClearScene(true);

		char* buffer = nullptr;
		uint size = App->file_system->ReadFromFile(tmp_str.c_str(), &buffer);

		if (buffer != nullptr && size > 0)
		{
			App->main_scene->DeSerializeScene(buffer);
			RELEASE_ARRAY(buffer);
		}
		else 
			TR_LOG("trFileLoader: Error reading scene from path: %s", tmp_str.c_str());
		
	}
}


bool trFileLoader::LoadMeshFile(const char* file_path)
{
	// Open file rquested file
	/*std::ifstream in_file;
	in_file.open(file_path, std::ifstream::in);

	// Check for errors
	if (in_file.fail())
	{
		TR_LOG("Unable to open file...");
		return false;
	}

	// Get size of file
	in_file.seekg(0, in_file.end);
	size_t size = in_file.tellg();
	in_file.seekg(0, in_file.beg);

	// Allocate memory for the buffer
	char* data = new char[size];
	char* cursor = data;

	// Copy file data into buffer and close file
	in_file.read(cursor, size);
	in_file.close();

	// amount of indices / vertices / colors / normals / texture_coords
	uint ranges[2];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	mesh_data->index_size = ranges[0];
	mesh_data->vertex_size = ranges[1];

	// Load indices
	cursor += bytes;
	bytes = sizeof(uint) * mesh_data->index_size;
	mesh_data->indices = new uint[mesh_data->index_size]; // CRASH HERE!!!
	memcpy(mesh_data->indices, cursor, bytes);

	// Load vertices
	cursor += bytes;
	bytes = sizeof(uint) * mesh_data->vertex_size;
	mesh_data->vertices = new float[mesh_data->vertex_size];
	memcpy(mesh_data->vertices, cursor, bytes);

	RELEASE_ARRAY(data);
	*/
	return true;
}