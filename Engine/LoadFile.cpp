#include "LoadFile.h"

#include <assert.h>
#include <vector>

namespace Engine
{
	std::vector<uint8_t> LoadFile(const std::string& i_Filename)
	{
		std::vector<uint8_t> Contents;

		if (!i_Filename.empty())
		{
			FILE* pFile = nullptr;

			errno_t fopenError = fopen_s(&pFile, i_Filename.c_str(), "rb");
			if (fopenError == 0)
			{
				assert(pFile != nullptr);

				int FileIOError = fseek(pFile, 0, SEEK_END);
				assert(FileIOError == 0);

				long FileSize = ftell(pFile);
				assert(FileSize >= 0);

				FileIOError = fseek(pFile, 0, SEEK_SET);
				assert(FileIOError == 0);

				Contents.reserve(FileSize);
				Contents.resize(FileSize);

				size_t FileRead = fread(&Contents[0], 1, FileSize, pFile);
				assert(FileRead == FileSize);

				fclose(pFile);
			}
		}

		return Contents;
	}

}