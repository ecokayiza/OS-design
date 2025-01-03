cmake_minimum_required(VERSION 3.5.0)
project(design VERSION 0.1.0 LANGUAGES C)



include_directories(${CMAKE_SOURCE_DIR}/include)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
set(SOURCE ${CMAKE_SOURCE_DIR}/source)
set(EXE ${CMAKE_SOURCE_DIR}/commands)


add_library(util SHARED ${CMAKE_SOURCE_DIR}/util/functions.c)
add_library(time SHARED ${CMAKE_SOURCE_DIR}/util/time_util.c)
add_library(file SHARED ${CMAKE_SOURCE_DIR}/util/file_util.c)

add_executable(CLS ${SOURCE}/CLS.c)
set_target_properties(CLS PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXE})
target_link_libraries(CLS util)


add_executable(DATE ${SOURCE}/DATE.c)
set_target_properties(DATE PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXE})
target_link_libraries(DATE util time)

add_executable(TIME ${SOURCE}/TIME.c)
set_target_properties(TIME PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXE})
target_link_libraries(TIME util time)

add_executable(FIND ${SOURCE}/FIND.c)
set_target_properties(FIND PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXE})
target_link_libraries(FIND util file)

add_executable(FINDSTR ${SOURCE}/FINDSTR.c)
set_target_properties(FINDSTR PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXE})
target_link_libraries(FINDSTR util file)

add_executable(COMP ${SOURCE}/COMP.c)
set_target_properties(COMP PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXE})
target_link_libraries(COMP util file)

add_executable(FC ${SOURCE}/FC.c)
set_target_properties(FC PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXE})
target_link_libraries(FC util file)

add_executable(EXIT ${SOURCE}/EXIT.c)
set_target_properties(EXIT PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXE})
target_link_libraries(EXIT util)

add_executable(HELP ${SOURCE}/HELP.c)
set_target_properties(HELP PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXE})
target_link_libraries(HELP util)

add_executable(DOSKEY ${SOURCE}/DOSKEY.c)
set_target_properties(DOSKEY PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXE})
target_link_libraries(DOSKEY util)

add_executable(MORE ${SOURCE}/MORE.c)
set_target_properties(MORE PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXE})
target_link_libraries(MORE util)