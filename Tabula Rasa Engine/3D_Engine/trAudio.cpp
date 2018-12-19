#include "trAudio.h"
#include "FirulWwise.h"

trAudio::trAudio()
{
}

trAudio::~trAudio()
{
}

bool trAudio::Start()
{
	TR_LOG("trAudio: Init with FirulWwise");

	FirulWwise::InitFWw();

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

	FirulWwise::CleanUpFWw();

	return true;
}

bool trAudio::InitWwise()
{
	return true;
}
