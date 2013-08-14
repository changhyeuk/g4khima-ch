cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

set(targetname "g4khima")
set(projname "proj${targetname}")
project(${projname})

# for geant4
set(libs)
find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})
list(APPEND libs ${Geant4_LIBRARIES})

# for root
if(WIN32)
  # on windows
  include_directories($ENV{ROOTSYS}/include)
  link_directories($ENV{ROOTSYS}/lib)
  list(APPEND libs "libGui.lib libCore.lib libCint.lib libRIO.lib libNet.lib libHist.lib libGraf.lib libGraf3d.lib libGpad.lib libTree.lib libRint.lib libPostscript.lib libMatrix.lib libPhysics.lib libMathCore.lib libThread.lib")
else()
  # on unix or mac
  set(CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
  find_package(ROOT REQUIRED)
  list(APPEND libs ${ROOT_LIBRARIES})
  include_directories(${ROOT_INCLUDE_DIR})
endif()
 
# for local files
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(${targetname} ${targetname}.cc ${sources} ${headers})
target_link_libraries(${targetname} ${libs})
add_custom_target(${projname} DEPENDS ${targetname})
