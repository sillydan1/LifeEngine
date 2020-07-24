# ============================================================================ #
#                                             ____ ______                      #
#                       CMAKE DEFINITIONS FOR LIFE ENGINE                      #
#                                                                              #
# ============================================================================ #
#  _______________________________________
# / You definitely intend to start living \
# \ sometime soon.                        /
#  ---------------------------------------
#         \   ^__^
#          \  (oo)\_______
#             (__)\       )\/\
#                 ||----w |
#                 ||     ||

if(MSVC)
	set(THREADING)
else(MSVC)
	set(THREADING pthread)
endif(MSVC)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -Wall -g -DWIN32")
else(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -Wall -g -Wno-int-to-pointer-cast -Wno-sign-compare")
endif(MSVC)

