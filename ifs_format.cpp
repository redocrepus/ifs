#include "ifs_format.h"
#include <Windows.h>
#include <regex>

namespace ifs {

    std::string quoted(std::string s) {
        return "\"" + s + "\"";
    }

    std::string quoted(fs::path p) {
        return "\"" + p.string() + "\"";
    }

    std::string trimWhiteSpaces(std::string s) {
        return std::regex_replace(s, std::regex("^\\s+|\\s+$"), "", std::regex_constants::format_default);
    }

    std::wstring wquoted(std::wstring s) {
        return L"\"" + s + L"\"";
    }

    std::wstring wquoted(fs::path p) {
        return L"\"" + p.wstring() + L"\"";
    }

    fs::path basePath(fs::path p)
    {
        static fs::path base;
        if (p != "___NOCHANGE")
            base = p;

        return base;
    }

    fs::path shrt(fs::path p)
    {
        fs::path pp = proximate(absolute(p), basePath());
        return pp;
    }

    bool GetCMDColors(short& color) {
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
            return false;

        color = info.wAttributes;      
        return true;
    }
    
    // This function was not tested.
    bool GetCMDColors(short& BG, short& FG) {
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
            return false;
        BG = info.wAttributes >> 8;
        FG = info.wAttributes &= 0xFF00;

        return true;
    }
}
