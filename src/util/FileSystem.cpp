#include "FileSystem.hpp"
#include <filesystem>
#include <life_core.h>
#ifdef WINDOWS
    #include <windows.h>
#else
    #include <limits.h>
    #include <unistd.h>
 #endif

namespace life {

    bool exepathgotten = false;
    std::string exepath;

#ifdef WIN32
    std::string getexepath() {
        if(exepathgotten) return exepath;
        char result[ MAX_PATH ];
        auto s = std::string( result, GetModuleFileName( NULL, result, MAX_PATH ) );
        exepath = s;
        exepathgotten = true;
        return s;
    }
#else
    std::string getexepath() {
        if(exepathgotten) return exepath;
        char result[ PATH_MAX ];
        ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
        auto s = std::string( result, (count > 0) ? count : 0 );
        s = s.substr(0, s.find_last_of('/'));
        exepathgotten = true;
        exepath = s;
        return s;
    }
#endif

    std::string getrespath() {
        LIFE_ASSERT(std::filesystem::exists(getexepath()+"/res"));
        return getexepath()+"/res";
    }

}

