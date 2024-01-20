cmake_minimum_required (VERSION 2.6)
project(Logger)

#project defined variables
SET(fw_name "vtz_logger") 
#Use this for profiling
#SET(${fw_name}_FLAGS "-g -fpermissive -fprofile-arcs -I/usr/include")
SET(dev_include include)
SET(ARCH "x64") 

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

#predefined variables
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${${fw_name}_FLAGS}" )
FOREACH(flag ${${fw_name}_FLAGS})
    message("auto FLAGS:" ${flag}) 
ENDFOREACH(flag)

# APPLE, UNIX AND NOT APPLE, WIN32, LINUX
if (LINUX) 
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed -Wl,--rpath=/usr/lib")
endif()

SET(CMAKE_C_FLAGS_DEBUG "-O0 -g") 

#variable check
IF ("${ARCH}" STREQUAL "x86") 
    message ("Platform:" ${ARCH}) 
    ADD_DEFINITIONS("-DTARGET")
ENDIF ("${ARCH}" STREQUAL "x86") 

# Compiler only cares this
# NOTE: Update this place if adding new compile or link flags
# Use this for profiling
# SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -std=c++0x -g -fprofile-arcs -ftest-coverage" )
SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -std=c++0x" )

#add_subdirectory(src)

# NOTE: Update this place if adding new include path.
SET(${fw_name}_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    /usr/include)

include_directories(${${fw_name}_INCLUDES})

# NOTE: Update this place if adding new library linking location.
SET(${fw_name}_LINKS 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    /usr/local/lib /usr/lib /usr/local/ssl/lib/)

#FIND_LIBRARY is better choice
link_directories(${${fw_name}_LINKS})

#if you make new line for each file generate error

SET(${fw_name}_SRCS 
    src/common/logging.cc
    )
message("[VC] " ${${fw_name}_INCLUDES})
#add_library(${fw_name} shared ${${fw_name}_SRCS})
add_library(${fw_name} SHARED src/common/logging.cc)

# NOTE: Update this place if adding new shared libraries.
# ISSUE(2022/10/04): openssl was compiled as below:
# 
# sudo ./config -fPIC shared
# history
# sudo make install
#
# also, export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/ssl/lib/pkgconfig/ to be pkg-config awared
# 
# also, set target link libraries
# libssl.so libcrypto.so
# 
# also, SET (pc_dependents "dbus-glib-1 libssl libcrypto")
# a lot to do with openssl !!!!
# https://www.miguelvallejo.com/tag/installing-openssl-1-0-2g-on-ubuntu/
# 
# real bad XXX.

if (LINUX)
#target_link_libraries(${fw_name} libm.so libpthread.so libgtest.so libgtest_main.so libssl.so libcrypto.so) 
endif()

#if (APPLE)
#target_link_libraries(${fw_name} libm.dylib libpthread.dylib libgtest.dylib libgtest_main.dylib)
#endif()

#install dev (headers) here why includes means /usr/local/include????
INSTALL(
        DIRECTORY ${dev_include}/ DESTINATION /usr/local/include/${fw_name}
        FILES_MATCHING
        PATTERN "*_private.h" EXCLUDE
        PATTERN "${dev_include}/*.h"
        PATTERN "${dev_include}/common/*.h"
        )


#TBD: Install at ${CMAKE_INSTALL_PREFIX} which is /usr/local so include and lib all means /usr/local as default
install(TARGETS ${fw_name} DESTINATION /usr/local/lib)

#TBD: pkg-config setting
SET(PC_NAME ${fw_name})
SET(PC_REQUIRED ${pc_dependents})
SET(PC_LDFLAGS -l${fw_name})

#CONFIGURE_FILE 
CONFIGURE_FILE(
    pkgconfig-template.pc.in
    ${CMAKE_CURRENT_SOURCE_DIR}/${fw_name}.pc
    @ONLY
)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${fw_name}.pc DESTINATION lib/pkgconfig)
