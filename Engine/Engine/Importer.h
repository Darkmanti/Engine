#pragma once

#include <stdint.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <sstream>

#include "GameObject.h"

namespace Importer
{
	uint32_t ImportObj(const char *fileName, GameObject &go);
};
