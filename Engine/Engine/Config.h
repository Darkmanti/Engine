#pragma once

#include <stdint.h>

extern uint16_t	windowX,
				windowY,
				windowWidth,
				windowHeight;

extern bool		fullscreen;

void SaveConfigInterface();
void LoadConfigInterface();
