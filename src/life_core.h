#ifndef LIFECORE_H
#define LIFECORE_H
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
#include <assert.h>
#include <thread>
#include <signal.h>
#include <iostream>

#ifdef WIN32
#define COLOR_RED(x) 		x
#define COLOR_YELLOW(x) 	x
#define COLOR_GREEN(x) 		x
#define COLOR_WHITE(x) 		x
#else
#define COLOR_ESC "\033["
#define COLOR_DONE COLOR_ESC "0m"
#define COLOR_RED(x) 		COLOR_ESC "1;31m" x COLOR_DONE
#define COLOR_YELLOW(x) 	COLOR_ESC "1;33m" x COLOR_DONE
#define COLOR_MAGENTA(x) 	COLOR_ESC "1;35m" x COLOR_DONE
#define COLOR_GREEN(x) 		COLOR_ESC "1;32m" x COLOR_DONE
#define COLOR_WHITE(x) 		COLOR_ESC "1;37m" x COLOR_DONE
#endif

#ifdef _MSC_VER
	#define DEBUG_BREAK __debugbreak()
#else
	#define DEBUG_BREAK raise(SIGTRAP)
#endif

#define BIT(x) (1<<x)
// Checks if the variadic arguments given is actually empty.
#define ISEMPTY(...)                                                    \
_ISEMPTY(                                                               \
          /* test if there is just one argument, eventually an empty    \
             one */                                                     \
          HAS_COMMA(__VA_ARGS__),                                       \
          /* test if _TRIGGER_PARENTHESIS_ together with the argument   \
             adds a comma */                                            \
          HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__),                 \
          /* test if the argument together with a parenthesis           \
             adds a comma */                                            \
          HAS_COMMA(__VA_ARGS__ (/*empty*/)),                           \
          /* test if placing it between _TRIGGER_PARENTHESIS_ and the   \
             parenthesis adds a comma */                                \
          HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__ (/*empty*/))      \
          )
#define PASTE5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define _ISEMPTY(_0, _1, _2, _3) HAS_COMMA(PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))
#define _IS_EMPTY_CASE_0001 ,
#define _ARG16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define HAS_COMMA(...) _ARG16(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define _TRIGGER_PARENTHESIS_(...) ,
#define LIFE_CAT(x, y) x ## y
#define LIFE_IF_0(t, f) f
#define LIFE_IF_1(t, f) t
#define GET_IF_(x,a,b) x(a,b)
#define GET_IF(x, iftrue, iffalse) GET_IF_(LIFE_CAT(LIFE_IF_, x), iftrue, iffalse)
/* = Window Manager API = - Deprecated
 * Naming is diffucult.
 * X11/X.Org is a display server and windowing system.
 * Wayland   is a display server and windowing system.
 * Desktop Window Manager is a window manager.
 * WindowServer is the OSX windowing system.
 * 
 * From now on, all of these are Window Managers
 * regardless of what they call them selves.
 */
// typedef enum {
// 	NO_WM			  	= 0, // Added for safety checks.
// 	LIFE_X11          	= 1,
// 	LIFE_WAYLAND      	= 2,
// 	LIFE_DWM          	= 3,
// 	LIFE_WINDOWSERVER 	= 4, // (Quartz?)
// 	LIFE_QT				= 5
// } WM_API;

typedef uint32_t uint;
typedef uint16_t ushort;
typedef uint8_t uchar;

extern const char* __life_ver__; // TODO: Get a good automatic versioning scheme

void __LIFE_LOG__(const char color, const char* prefix, const char* postfix, const char* pFileName, int line, const char* format, ...);
std::shared_ptr<std::ostream> set_output_stream(std::shared_ptr<std::ostream> stream);
void SetThreadName(std::thread* thread, const std::string& name);
uint GetCoreCount();

template<typename Out>
void split(const std::string &s, char delim, Out result);
std::vector<std::string> split(const std::string &s, char delim);

void ltrim(std::string &s);
void rtrim(std::string &s);
void trim(std::string &s);

// functions overloaded with _A and _0 based on arguments
#define A0_OVERLOADED_FUNC(func, _1, ...) GET_IF(ISEMPTY(__VA_ARGS__), func##_0(_1), func##_A(_1,__VA_ARGS__))

// ERROR_A means errors with arguments
#define LIFE_ERR_A(msg, ...) 	 __LIFE_LOG__(12, COLOR_RED(    "[ERR]:\t"),  "", __FILE__, __LINE__,  msg, __VA_ARGS__)
#define LIFE_ERR_0(msg) 		 __LIFE_LOG__(12, COLOR_RED(    "[ERR]:\t"),  "", __FILE__, __LINE__,  msg)
#define LIFE_WRN_A(msg, ...)  	 __LIFE_LOG__(14, COLOR_YELLOW( "[WRN]:\t"),  "", "", 		 -1, 	   msg, __VA_ARGS__)
#define LIFE_WRN_0(msg) 		 __LIFE_LOG__(14, COLOR_YELLOW( "[WRN]:\t"),  "", "", 		 -1, 	   msg)
#define LIFE_DBG_A(msg, ...) 	 __LIFE_LOG__(13, COLOR_MAGENTA("[DBG]:\t"),  "", __FILE__, __LINE__,  msg, __VA_ARGS__)
#define LIFE_DBG_0(msg) 		 __LIFE_LOG__(13, COLOR_MAGENTA("[DBG]:\t"),  "", __FILE__, __LINE__,  msg)
#define LIFE_LOG_A(msg, ...) 	 __LIFE_LOG__(15, COLOR_WHITE(  "[LOG]:\t"),  "", "", 		 -1, 	   msg, __VA_ARGS__)
#define LIFE_LOG_0(msg) 		 __LIFE_LOG__(15, COLOR_WHITE(  "[LOG]:\t"),  "", "", 		 -1, 	   msg)
#define LIFE_OGL_ERR_A(msg, ...) __LIFE_LOG__(14,COLOR_RED(  "[OpenGL]: "),   "", "",   	 -1, 	   msg, __VA_ARGS__)
#define LIFE_OGL_ERR_0(msg, ...) __LIFE_LOG__(14,COLOR_RED(  "[OpenGL]: "),   "", "",   	 -1, 	   msg)
// Macro interfaces
void life_brkpntr(); // Only used for debug purposes
#ifdef LIFE_ENABLE_ERR_BREAKPOINTS
  #define LIFE_ERR(msg,...) 		A0_OVERLOADED_FUNC(LIFE_ERR, msg, __VA_ARGS__); life_brkpntr()
#else
  #define LIFE_ERR(msg,...) 		A0_OVERLOADED_FUNC(LIFE_ERR, msg, __VA_ARGS__)
#endif
#ifdef LIFE_ENABLE_OGLERR_BREAKPOINTS
  #define LIFE_OGL_ERR(msg,...) 	A0_OVERLOADED_FUNC(LIFE_OGL_ERR, msg, __VA_ARGS__); life_brkpntr()
#else
  #define LIFE_OGL_ERR(msg,...) 	A0_OVERLOADED_FUNC(LIFE_OGL_ERR, msg, __VA_ARGS__)
#endif
#define LIFE_DBG(msg,...) 		A0_OVERLOADED_FUNC(LIFE_DBG, msg, __VA_ARGS__)
#define LIFE_WARN(msg,...) 		A0_OVERLOADED_FUNC(LIFE_WRN, msg, __VA_ARGS__)
#define LIFE_LOG(msg,...) 		A0_OVERLOADED_FUNC(LIFE_LOG, msg, __VA_ARGS__)


#ifdef LIFE_ENABLE_ASSERTS
	#define LIFE_ASSERT_A(x, msg) 	{ if (!(x)) { LIFE_ERR_A("Assertion failed: %s", msg); DEBUG_BREAK; } }
	#define LIFE_ASSERT_0(x) 		{ if (!(x)) { LIFE_ERR_A("Assertion failed: %s", #x);   DEBUG_BREAK; } }
	#define LIFE_ASSERT(x,...) 		A0_OVERLOADED_FUNC(LIFE_ASSERT, x, __VA_ARGS__)
#else
	#define LIFE_ASSERT(...) // Ignore asserts
#endif

#endif //LIFESTD_H
