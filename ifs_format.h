#pragma once
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

#define IFS_EXCEPTION_MSG(msg) ((std::string)static_cast<const char *>(__FUNCTION__) + ": " + msg)

namespace ifs {
	std::string quoted(std::string s);

	std::string quoted(fs::path p);

	std::string trimWhiteSpaces(std::string s);

	std::wstring wquoted(std::wstring s);

	std::wstring wquoted(fs::path p);

	fs::path basePath(fs::path p="___NOCHANGE");

	fs::path shrt(fs::path p);

}