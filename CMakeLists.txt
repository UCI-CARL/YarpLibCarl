cmake_minimum_required(VERSION 3.20)

project(YARP_carl
  LANGUAGES CXX
  VERSION 1.0.0)
 
set(CMAKE_CXX_STANDARD 11)


include(FeatureSummary)

find_package(YCM REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

if(MSVC)
  set(CMAKE_DEBUG_POSTFIX "d")  
  get_property(_USER_CMAKE_CXX_FLAGS CACHE CMAKE_CXX_FLAGS PROPERTY VALUE)
  string(FIND ${_USER_CMAKE_CXX_FLAGS} "/MP" _MSVC_MP_IDX)
  if(_MSVC_MP_IDX EQUAL -1)
    message("enforce parallel build")
    set(CMAKE_CXX_FLAGS "${_USER_CMAKE_CXX_FLAGS} /MP" CACHE STRING "enforce parallel build" FORCE)    
  endif()
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


find_package(YARP 3.4 COMPONENTS os sig dev REQUIRED)
set(CMAKE_C_FLAGS "${YARP_C_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${YARP_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")

find_package(ACE 6.5)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES)

add_subdirectory(libYARP_carl)


list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/../YARP/cmake)
                                                                                              
include(YarpInstallBasicPackageFiles)
  
yarp_install_basic_package_files(YARP_carl
                                 DEPENDENCIES ${YARP_carl_PUBLIC_DEPS}
                                 PRIVATE_DEPENDENCIES ${YARP_carl_PRIVATE_DEPS})

