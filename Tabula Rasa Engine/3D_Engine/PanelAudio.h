#ifndef __PANELAUDIO_H__
#define __PANELAUDIO_H__

#include "Panel.h"
#include "ImGui/imgui.h"
#include "trDefs.h"

#include "trTimer.h"

class ComponentAudio;

class PanelAudio : public Panel
{
public:
	PanelAudio();
	virtual ~PanelAudio();

	void Draw() override;

private:
	const char* go_names[100];
	int current_emitter = 0;
	int current_listener = 0;
	int bank_current = 0;

	bool emitter = true;
	bool listener = false;
	int type_current = 0;

	float game_volume = 50.0f;
	float music_volume = 50.0f;
	float spatial_volume = 50.0f;

private:
	ComponentAudio* AddComponentAudio(int current);
};
#endif