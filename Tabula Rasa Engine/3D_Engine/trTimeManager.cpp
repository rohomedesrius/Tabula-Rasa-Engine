#include "trTimeManager.h"


trTimeManager::trTimeManager() { }

trTimeManager::~trTimeManager() { }

bool trTimeManager::Start() { return true; }


// ---------- Getters ----------

double trTimeManager::ReadGameClockSec() const
{
	return game_sec_time;
}

double trTimeManager::ReadRealTimeClockSec() const
{
	return real_sec_time;
}

float trTimeManager::GetGameClockTimeScale() const
{
	return time_scale;
}

uint trTimeManager::GetFrameCount() const
{
	return frame_count;
}

float trTimeManager::GetGameDt() const
{
	return dt;
}

float trTimeManager::GetRealTimeDt() const
{
	return real_time_dt;
}

// ---------- Setters ----------

void trTimeManager::SetGameClockTimeScale(float time_scale)
{
	this->time_scale = time_scale;
}

void trTimeManager::UpdateGameClock(float dt)
{
	if (is_game_clock_paused)
		this->dt = 0.0f;
	else
		this->dt = dt;

	game_sec_time += (double)(this->dt * time_scale);

	frame_count++;
}

void trTimeManager::UpdateRealTimeClock(float dt)
{
	real_time_dt = dt;
	real_sec_time += (double)(real_time_dt);
}

// ---------- Time management ----------

void trTimeManager::PauseGameClock()
{
	is_game_clock_paused = true;
}

void trTimeManager::ReStartGameClock()
{
	is_game_clock_paused = false;
}