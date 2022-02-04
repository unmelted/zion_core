## 2022.01.15 
## created by Kelly (ekma@4dreplay.com)
##

cmake_minimum_required(VERSION 3.7)
project(zion)
set (PLATFORM MACM1) # available : MACM1, MACINTEL, WIN, ARM, LINUX
set (GPUMODE off )
message(${GPUMODE})
message(${PLATFORM})

if( ${PLATFORM} STREQUAL "WIN" )
    set(OpenCV_INCLUDE_DIRS ${PRJ_PATH}/3rd/opencv4.5.3/include/)
    set(OpenCV_LIBS ${PRJ_PATH}/3rd/opencv4.5.3/lib/)
else()
    find_package( OpenCV REQUIRED )
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-invalid-source-encoding -Wno-deprecated-register -Wno-writable-strings -Wno-parentheses -I3rd/ -Isrc/ -Isrc/daemon/ -Isrc/common/ -Isrc/darknet -Isrc/dove -O2 -g")


add_definitions(-D_DEBUG)
add_definitions(-D_IMGDEBUG)
add_definitions(-DOPENCV)

if(APPLE)
    add_definitions(-D_MAC_)
else ()
    add_definitions(-D_WIN_)
endif()

set(PRJ_PATH        ${CMAKE_CURRENT_SOURCE_DIR})
set(RJSON_INCLUDE_DIR   ${PRJ_PATH}/3rd/rapidjson)
set(ZL_INCLUDE_DIR      ${PRJ_PATH}/3rd/ZLToolKit/include)
set(ZL_LIB              ${PRJ_PATH}/3rd/ZLToolKit/lib/${PLATFORM}/libZLToolKit.dylib)
set(SPD_INCLUDE_DIR     ${PRJ_PATH}/3rd/spdlog/include)
set(SPD_LIB             ${PRJ_PATH}/3rd/spdlog/lib/${PLATFORM}/libspdlog.a)
set(DARKNET_PATH        ${PRJ_PATH}/3rd/darknet)
set(DARKNET_LIB         ${DARKNET_PATH}/${PLATFORM}/libdarknet.dylib)
set(GSL_INCLUDE_DIR     ${PRJ_PATH}/3rd/gsl)
set(GSL_LIB             ${PRJ_PATH}/3rd/gsl/lib/${PLATFORM}/libgsl.dylib)
message(${GSL_LIB})
file(GLOB PRJ_SRC   ${PRJ_PATH}/src/*.cpp
                    ${PRJ_PATH}/src/common/*.cpp                    
                    ${PRJ_PATH}/src/daemon/*.cpp
                    ${PRJ_PATH}/src/dove/*.cpp
)

if( ${PLATFORM} STREQUAL "MACsM1" )
    list(REMOVE_ITEM PRJ_SRC ${PRJ_PATH}/src/FrTrans.cpp)
endif()

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
target_link_libraries(zion ${GSL_LIB} ${ZL_LIB} ${OpenCV_LIBS}  ${DARKNET_LIB} ${SPD_LIB})
