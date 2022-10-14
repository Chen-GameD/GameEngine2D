#pragma once

#include <functional>
#include <string>

#include "JobStatus.h"
#include "HashedString.h"
#include "Events.h"

namespace Engine
{
	namespace JobSystem
	{
		void Init();

		HashedString GetDefaultQueue();

		struct JobData
		{
			HashedString QueueName;
			JobStatus* pJobStatus;

			JobData(const HashedString& i_QueueName = GetDefaultQueue(), JobStatus* i_pJobStatus = nullptr) :
				QueueName(i_QueueName),
				pJobStatus(i_pJobStatus)
			{}
		};

		inline const char* GetDefaultQueueName() noexcept {
			static char DefaultQueueName[] = "Default";
			return DefaultQueueName;
		}

		inline HashedString GetDefaultQueue() {
			return HashedString(GetDefaultQueueName());
		}
		HashedString CreateQueue(const std::string& i_Name, unsigned int i_numRunners);

		void RunJob(const std::string& i_JobName, std::function<void()> i_JobFunction, const JobData& i_JobData);
		bool HasJobs(const HashedString& i_QueueName);

		void RequestShutdown();
		bool ShutdownRequested();

	} // namespace JobSystem
} // namespace Engine
