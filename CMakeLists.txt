cmake_minimum_required(VERSION 3.7)
project(CSCI520Proj3)

#########################################################
# SET COMPILER
#########################################################
set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# FIND GNU Scientific Library
#########################################################

find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})
link_directories(${GSL_LIBRARY_DIRS})
add_definitions(${GSL_DEFINITIONS})
if(NOT GSL_FOUND)
    message(ERROR " GSL not found!")
endif(NOT GSL_FOUND)

#########################################################
# SET EXE
#########################################################
SET(CHAIN_SOURCE
        main.cpp
        chain.cpp



        )

SET(CHAIN_HEADERS


        openGL-headers.h chain.h)

#########################################################
# ADD EXECUTABLES
#########################################################
add_executable(chain ${CHAIN_SOURCE} ${CHAIN_HEADERS})

#########################################################
# LINK LIBRARIES
#########################################################
target_link_libraries(chain ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GSL_LIBRARIES})