#include "trAudio.h"
#include "FirulWwise.h"
#include "trDefs.h"

trAudio::trAudio()
{
}

trAudio::~trAudio()
{
}

bool trAudio::Start()
{
	TR_LOG("trAudio: Init with FirulWwise");
	
	const size_t size = strlen(A_SOUNDBANKS_DIR) + 1;
	wchar_t* banks_directory = new wchar_t[size];
	mbstowcs(banks_directory, A_SOUNDBANKS_DIR, size);
	
	FirulWwise::InitFWw(banks_directory);

	return true;
}

bool trAudio::PreUpdate(float dt)
{
	return true;
}

bool trAudio::Update(float dt)
{
	return true;
}

bool trAudio::PostUpdate(float dt)
{
	//FirulWwise::RenderFirul();
	return true;
}
bool trAudio::CleanUp()
{
	TR_LOG("trAudio: CleanUp");

	RELEASE(banks_directory);

	FirulWwise::CleanUpFWw();

	return true;
}

bool trAudio::InitWwise()
{
	return true;
}
