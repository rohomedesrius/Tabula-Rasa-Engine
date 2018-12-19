#ifndef __PANELAUDIO_H__
#define __PANELAUDIO_H__

#include "Panel.h"
#include "ImGui/imgui.h"
#include "trDefs.h"

#include <list>

#include "trTimer.h"

class PanelAudio : public Panel
{
public:
	PanelAudio();
	virtual ~PanelAudio();

	void Draw() override;

private:
	const char* go_names[];
};
#endif