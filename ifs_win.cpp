#include "ifs_win.h"
#include <Windows.h>
#include <stdio.h>
#include <stdexcept>
#include "ifs_format.h"
#include "ifs_time.h"
#include "ifs_fs.h"
#include <sstream>

constexpr auto IFS_ERR_MSG_LEN = 1024;

void ifs::win::disableQuickEdit()
{
    HANDLE hInput;
    DWORD prev_mode;
    DWORD err = GetLastError();
    char errMsg[IFS_ERR_MSG_LEN];

    hInput = GetStdHandle(STD_INPUT_HANDLE);
 
    if (!GetConsoleMode(hInput, &prev_mode)) {
        snprintf(errMsg, sizeof(errMsg), "GetConsoleMode failed. Last error code=%d", GetLastError());
        throw std::runtime_error(IFS_EXCEPTION_MSG(errMsg));
    }
    if (!SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE))) {
        snprintf(errMsg, sizeof(errMsg), "SetConsoleMode failed. Last error code=%d", GetLastError());
        throw std::runtime_error(IFS_EXCEPTION_MSG(errMsg));
    }
}

fs::path ifs::win::currentExePath() {
    char szExeFilePath[MAX_PATH]{}; // TODO: EASY: make it static
    DWORD dwRet = GetModuleFileNameA(GetModuleHandle(NULL), szExeFilePath, sizeof szExeFilePath);
    if (!dwRet)
        throw std::runtime_error(IFS_EXCEPTION_MSG("Couldn't get exe path: GetModuleFileNameA failed."));
    return szExeFilePath;
}

std::string ifs::win::escQuotedPathString(fs::path path)
{
    // Create quoted paths with double backslashes. There could be problems with other special characters. For possible solution see: https://stackoverflow.com/questions/39228912/stdregex-escape-special-characters-for-use-in-regex
    std::stringstream sv;
    sv << path;
    return sv.str();
}

fs::path ifs::win::getInstallPath(std::string programName) {

    HKEY uninstallHKey;
    RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", 0, KEY_READ, &uninstallHKey);

    for (DWORD i = 0; ; i++) {
        // subKeyName gets the name of the key inside uninstallHKey at the current index
        CHAR subKeyName[255] = {};
        DWORD subKeyNameSize = sizeof(subKeyName);
        LONG result = RegEnumKeyExA(uninstallHKey, i, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL);

        // Failed to read key (Finished looping)
        if (result != ERROR_SUCCESS) {
            break;
        }

        // programHKEy gets the registry key that is uninstallHKey\subKeyName
        HKEY programHKEy;
        result = RegOpenKeyExA(uninstallHKey, subKeyName, 0, KEY_READ, &programHKEy);

        // Failed to read key (shouldn't happen here)
        if (result != ERROR_SUCCESS) {
            // Closing the registry key
            RegCloseKey(programHKEy);
            break;
        }

        // Reading the value of DisplayName in programHKEy
        CHAR nameBuffer[512] = {};
        DWORD nameBufferSize = sizeof(nameBuffer);
        result = RegQueryValueExA(programHKEy, "DisplayName", 0, NULL, (LPBYTE)nameBuffer, &nameBufferSize);

        // Failed to read key (no DisplayName)
        if (result != ERROR_SUCCESS) {
            continue;
        }

        // Found the key with our programName
        if (nameBuffer == programName) {
            // Reading the value of InstallLocation in programHKEy
            CHAR installBuffer[512] = {};
            DWORD installBufferSize = sizeof(installBuffer);
            result = RegQueryValueExA(programHKEy, "InstallLocation", 0, NULL, (LPBYTE)installBuffer, &installBufferSize);

            // Failed to read key (no InstallLocation for some reason)
            if (result != ERROR_SUCCESS)
                break;

            // Closing the registry key
            RegCloseKey(uninstallHKey);

            // Returning the installation path of the program
            return fs::path(installBuffer);
        }
    }

    // Closing the registry key
    RegCloseKey(uninstallHKey);

    throw std::runtime_error("Didn't find install location for program: " + programName);
}

void ifs::win::command::run() {

    int err = std::system(ifs::quoted(this->str()).c_str());

    if (err)
        throw cmd_error(this->str(), err, IFS_EXCEPTION_MSG("failed."));

    // Reset the string stream.  See: https://stackoverflow.com/questions/20731/how-do-you-clear-a-stringstream-variable 
    this->str("");
    this->clear();
}

std::string ifs::win::command::runAndGetOutput() {

//    std::string tempFileName = "ifs-win-cmd_" + ifs::timestamp("%H%M%S") + ".txt";
//    fs::path tempFilePath = fs::temp_directory_path() / tempFileName;
    fs::path tempFilePath = fs::temp_directory_path() / "ifs-win-cmd.tmp"; // no need to keep a log of temp files. we can overwrite the previous one if exists. Arkady.

    int err = std::system(ifs::quoted(this->str() + " > " + ifs::win::escQuotedPathString(tempFilePath)).c_str());

    if (err)
        throw cmd_error(this->str(), err, IFS_EXCEPTION_MSG("failed."));

    // Reset the string stream.  See: https://stackoverflow.com/questions/20731/how-do-you-clear-a-stringstream-variable 
    this->str("");
    this->clear();

    return ifs::ReadFileToString(tempFilePath);
}

