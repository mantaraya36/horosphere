#CMAKE FOR VERSOR+ALLOSPHERE DEMOS

cmake_minimum_required(VERSION 2.8)

project(HOROSPHERE)

#set(ALLOSPHERE 0 CACHE STRING "build for allosphere")
#set(BUILD_ALLOUTIL 1)
set(BUILD_ALLOGLV 1)
set(NO_EXAMPLES 1)

#get linker info from AlloSystem make system, but do not make all examples
add_subdirectory(../AlloSystem AlloSystem)

#NOTE: Was necessary to set_target_property of ALLOCORE_LINK_LIBRARIES within alloutil
#Would be nice to just grab AlloSystem Libraries (ALLOSYSTEM_LIBRARIES)
get_target_property(ALLOCORE_LINK_LIBRARIES allocore ALLOCORE_LINK_LIBRARIES)
get_target_property(ALLOUTIL_LINK_LIBRARIES alloutil ALLOUTIL_LINK_LIBRARIES)

#print out linker libs
#message("ALLOCORE_LINK_LIBRARIES: " ${ALLOCORE_LINK_LIBRARIES})
#message("ALLOUTIL_LINK_LIBRARIES: " ${ALLOUTIL_LINK_LIBRARIES})

#local include for this particular project space
include_directories(/usr/local/include/)
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/demos/)
include_directories(${PROJECT_SOURCE_DIR}/../versor/)
include_directories(${PROJECT_SOURCE_DIR}/../versor/vsr/)
include_directories(${PROJECT_SOURCE_DIR}/../GLV/)
include_directories(${PROJECT_SOURCE_DIR}/../versor/ext/gfx/)
include_directories(${PROJECT_SOURCE_DIR}/../versor/ext/gfx/gfx/)
include_directories(${PROJECT_SOURCE_DIR}/../Gamma/)
include_directories(${PROJECT_SOURCE_DIR}/../cuttlebone/)

#would be nice to just grab AlloSystem Include Directories (ALLOSYSTEM_INCLUDE_DIR)
include_directories(${AlloSystem_SOURCE_DIR}/allocore/)
include_directories(${AlloSystem_SOURCE_DIR}/alloutil/)
include_directories(${AlloSystem_SOURCE_DIR}/alloGLV/)

#library search directories
link_directories(/usr/local/lib/)
link_directories(${PROJECT_SOURCE_DIR}/../versor/build/lib/)
link_directories(${PROJECT_SOURCE_DIR}/../GLV/build/lib/)
link_directories(${PROJECT_SOURCE_DIR}/../Gamma/build/lib/)
link_directories(${AlloSystem_SOURCE_DIR}/build/lib)

if(${BUILDING_FOR_ALLOSPHERE})
link_directories(/usr/lib/x86_64-linux-gnu/)
else()
#include_directories($ENV{HOME}/code/libcxx/include)#/usr/local/include/libcxx/)#$
#link_directories($ENV{HOME}/code/libcxx/lib)#/usr/local/lib/libcxx/) #
endif()


get_directory_property(LD LINK_DIRECTORIES)
#message("LINK_DIRECTORIES" ${LD})

if(${BUILDING_FOR_ALLOSPHERE})
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D __allosphere__")
endif()

#the subdirectory of demos
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

add_subdirectory(${DIR})
message("now type 'make'")
