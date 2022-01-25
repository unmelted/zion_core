## 2022.01.15 
## created by Kelly (ekma@4dreplay.com)
##

cmake_minimum_required(VERSION 3.7)
#set(CMAKE_C_COMPILER "clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang")

project(zion)
set (PLATFROM MAC) # available : WIN, ARM, LINUX
set(CMAKE_CXX_STANDARD 17)
find_package( OpenCV REQUIRED )

#if(NOT TARGET spdlog)
    # Stand-alone build
#    find_package(spdlog REQUIRED)
#endif()

set(CMAKE_CXX_FLAGS "-Wno-invalid-source-encoding -Wno-deprecated-register -Wno-writable-strings -Wno-parentheses -I3rd/ -Isrc/ -Isrc/daemon/ -Isrc/common/ -Isrc/darknet -Isrc/dove -O2 -g")

set(GSL_INCLUDE_DIR "/usr/local/include")
set(GSL_LIB_DIR "/usr/local/lib/libgsl.dylib")

add_definitions(-D_DEBUG)
add_definitions(-D_IMGDEBUG)
add_definitions(-DOPENCV)

if(APPLE)
    add_definitions(-D_MAC_)
else ()
    add_definitions(-D_WIN_)
endif()

set(PRJ_PATH        ${CMAKE_CURRENT_SOURCE_DIR})
set(ZL_INCLUDE_DIR      ${PRJ_PATH}/3rd/ZLToolKit/include)
set(ZL_LIB              ${PRJ_PATH}/3rd/ZLToolKit/lib/libZLToolKit.dylib)
set(SPD_INCLUDE_DIR     ${PRJ_PATH}/3rd/spdlog/include)
set(SPD_LIB             ${PRJ_PATH}/3rd/spdlog/lib/libspdlog.a)
set(RJSON_INCLUDE_DIR   ${PRJ_PATH}/3rd/rapidjson)
set(DARKNET_PATH        ${PRJ_PATH}/3rd/darknet)
set(DARKNET_LIB         ${DARKNET_PATH}/libdarknet.dylib)

file(GLOB PRJ_SRC   ${PRJ_PATH}/src/*.cpp
                    ${PRJ_PATH}/src/common/*.cpp                    
                    ${PRJ_PATH}/src/daemon/*.cpp
                    ${PRJ_PATH}/src/dove/*.cpp
)
list(REMOVE_ITEM PRJ_SRC ${PRJ_PATH}/src/FrTrans.cpp)
file(GLOB PRJ_INC      ${PRJ_PATH}/src/*.hpp
                       ${PRJ_PATH}/src/common/*.hpp
                       ${PRJ_PATH}/src/daemon/*.h
                       ${PRJ_PATH}/src/dove/*.hpp
                       ${DARKNET_PATH}/darknet.h
                       ${DARKNET_PATH}/yolo_v2_class.hpp                       
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${GSL_INCLUDE_DIR} ${PRJ_INC} ${OpenCV_INCLUDE_DIRS} ${ZL_INCLUDE_DIR} ${SPD_INCLUDE_DIR})
message(${OpenCV_INCLUDE_DIRS})
add_executable(zion ${PRJ_SRC})
target_link_libraries(zion ${GSL_LIB_DIR} ${ZL_LIB} ${OpenCV_LIBS}  ${DARKNET_LIB} ${SPD_LIB})
