#include <life_core.h>
#include <cstdarg>
#include <thread>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <cctype>
#include <locale>
#include <fstream>
#include <iostream>
#include <utility>

#define SNPRINTF snprintf
#define VSNPRINTF vsnprintf

#ifdef WIN32
#include <windows.h>
#include <iostream>
#define LIFE_DEFAULT_COLOR 15 // White
HANDLE hConsole = nullptr;
#endif

#define MAX_MESSAGE_SIZE 1024

const char* __life_ver__ = "Life Engine v3.3 (alpha)";

// a custom deleter that does nothing (no sure why the standard library doesn't have one of these):
struct null_deleter {
    template <typename T> void operator()(T*) const noexcept { };
};

std::shared_ptr<std::ostream> global_out = std::shared_ptr<std::ostream>(&std::cout, null_deleter());

std::shared_ptr<std::ostream> set_output_stream(std::shared_ptr<std::ostream> stream) {
    global_out = std::move(stream);
    return global_out;
}

void __LIFE_LOG__(const char color, const char* prefix, const char* postfix, const char* pFileName, int line, const char* format, ...) {
	char msg[MAX_MESSAGE_SIZE];    
    va_list args;
    va_start(args, format);
    VSNPRINTF(msg, sizeof(msg), format, args);
    va_end(args);
#ifdef WIN32 
	if (!hConsole) {
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	SetConsoleTextAttribute(hConsole, color);
	std::cout << prefix;
	SetConsoleTextAttribute(hConsole, LIFE_DEFAULT_COLOR);
    char msg2[MAX_MESSAGE_SIZE];
    if(line < 0) {
		_snprintf_s(msg2, sizeof(msg2), "%s%s%s\n", pFileName, msg, postfix);
	} else {
		_snprintf_s(msg2, sizeof(msg2), "%s:%d: %s%s\n", pFileName, line, msg, postfix);
	}
    // MessageBoxA(NULL, msg2, NULL, 0);
	fprintf(stdout, msg2); // TODO: output stream should be variable. (Maybe we want to log to a file, or an internal stream for ImGUI logging)
#else
	if(line < 0) {
		// sprintf(std::cout, "%s%s%s%s\n", prefix, pFileName, msg, postfix);
        *global_out << prefix << pFileName << msg << postfix << "\n";
	} else {
		// sprintf(std::cout, "%s%s:%d: %s%s\n", prefix, pFileName, line, msg, postfix);
        *global_out << prefix << pFileName << ":" << line << ": " << msg << postfix << "\n";
	}
#endif
}

void SetThreadName(std::thread* thread, const std::string& name) {
#ifndef WIN32
	auto handle = thread->native_handle();   // Grab thread handle
	pthread_setname_np(handle, name.c_str()); // Set the posix thread name
#else
    LIFE_ERR("SetThreadName only works on Linux right now!");
#endif
}

uint GetCoreCount() {
	return std::thread::hardware_concurrency();
}

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

// trim from start (in place)
void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

void life_brkpntr() {
    return;
}

