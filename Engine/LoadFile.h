#pragma once

#include <vector>
#include <string>
#include <stdint.h>

namespace Engine
{
	std::vector<uint8_t> LoadFile(const std::string& i_FileName);
}