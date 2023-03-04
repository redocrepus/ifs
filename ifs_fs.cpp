#include "ifs_fs.h"

#include <stdlib.h>
#include <stdio.h>
#include <shlobj_core.h>
#include <direct.h>

namespace ifs {
    void Copy(const fs::path from, const fs::path to) {
        try
        {
            fs::copy(from, to);
        }
        catch (std::exception e) {
            throw std::runtime_error(IFS_EXCEPTION_MSG("could not copy: " + ifs::quoted(fs::absolute(from)) + " to : " + ifs::quoted(fs::absolute(to)) + " Error: " + e.what()));
            // TODO: NOW: handle status. let the user know if one of the sources was not found and which one
        }

    }

    std::string ReadFileToString(const fs::path& fsPath)
    {
        // Source: https://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html

        std::ifstream in(fsPath, std::ios::in | std::ios::binary);
        if (in)
        {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return(contents);
        }
        throw(
            std::runtime_error(std::string("Error reading file: ") + fsPath.string() + std::string(" . errno: ") + std::to_string(errno) // TODO: handle unicode
            ));

    }

    std::string GetEnvVar(std::string varName) {
            // Source: https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/getenv-s-wgetenv-s?view=msvc-170

        size_t requiredSize;

        getenv_s(&requiredSize, NULL, 0, varName.c_str());
        if (0 == requiredSize)
            throw std::runtime_error(std::string("variable ") + quoted(varName) + " is not defined"); // TODO: Use more specific exception

        char* pchVal = new char[requiredSize];// Make sure you release the memory.

        // Get the value of the environment variable.
        getenv_s(&requiredSize, pchVal, requiredSize, varName.c_str());
        std::string sVal(pchVal);

        delete[] pchVal;
        pchVal = NULL; // safety. prevents deleting it twice.

        return sVal;
    }
}