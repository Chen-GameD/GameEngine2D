#pragma once

#include "JobSystem.h"
#include "Events.h"
#include "HashedString.h"

#include <functional>
#include <string>
#include <vector>

namespace Engine
{
	void ProcessFileAsync(const std::string& i_Filename, std::function<void(std::vector<uint8_t>)> i_Processor, JobSystem::JobData i_JobData = JobSystem::JobData());

} // namespace Engine