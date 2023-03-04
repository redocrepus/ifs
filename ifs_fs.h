#pragma once


#include <filesystem>
#include <fstream>
#include <string>
#include "ifs_format.h"

namespace fs = std::filesystem;

namespace ifs {
	void Copy(const fs::path from, const fs::path to); // Throws

	std::string ReadFileToString(const fs::path& fsPath);


	std::string GetEnvVar(std::string varName);

}

