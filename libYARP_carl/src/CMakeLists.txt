 
add_library(YARP_carl SHARED)
add_library(YARP::YARP_carl ALIAS YARP_carl)


set(YARP_carl_HDRS  yarp/carl/AceUtils.h
                  yarp/carl/all.h
                  yarp/carl/api.h
                  yarp/carl/BottleDecoder.h
                  yarp/carl/BottleEncoder.h                  
                  yarp/carl/StlUtils.h)

set(YARP_carl_SRCS  yarp/carl/AceUtils.cpp
                  yarp/carl/BottleDecoder.cpp
                  yarp/carl/BottleEncoder.cpp
                  yarp/carl/Dummy.cpp
                  yarp/carl/StlUtils.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
             PREFIX "Source Files"
             FILES ${YARP_carl_SRCS})
             
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
             PREFIX "Header Files"
             FILES ${YARP_carl_HDRS})

target_sources(YARP_carl PRIVATE ${YARP_carl_SRCS}
                                ${YARP_carl_HDRS})


FIND_PACKAGE (Boost 1.74 REQUIRED COMPONENTS date_time container )
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

find_package(CARLcpp REQUIRED)
message("CARLCPP_INCLUDE_DIR: ${CARLCPP_INCLUDE_DIR}")
include_directories(${CARLCPP_INCLUDE_DIR})

target_include_directories(YARP_carl PUBLIC 
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


#target_compile_features(YARP_carl PUBLIC cxx_std_14)
target_compile_features(YARP_carl PUBLIC cxx_std_11)

target_link_libraries(YARP_carl PUBLIC YARP::YARP_os)
list(APPEND YARP_nc_PUBLIC_DEPS YARP_dev YARP_os)

#if(YARP_HAS_ACE)
  target_compile_definitions(YARP_carl PRIVATE YARP_HAS_ACE)
  target_link_libraries(YARP_carl PRIVATE ACE::ACE)
  list(APPEND YARP_carl_PRIVATE_DEPS ACE)
#endif()


set_property(TARGET YARP_carl PROPERTY PUBLIC_HEADER ${YARP_carl_HDRS})
set_property(TARGET YARP_carl PROPERTY VERSION ${YARP_VERSION_SHORT})
set_property(TARGET YARP_carl PROPERTY SOVERSION ${YARP_SOVERSION})
set_property(TARGET YARP_carl PROPERTY FOLDER "Libraries")

install(TARGETS YARP_carl
        EXPORT YARP_carl
        RUNTIME
          DESTINATION "${CMAKE_INSTALL_BINDIR}"
          COMPONENT YARP_carl
        LIBRARY
          DESTINATION "${CMAKE_INSTALL_LIBDIR}"
          COMPONENT YARP_carl
          NAMELINK_COMPONENT YARP_carl-dev
        ARCHIVE
          DESTINATION "${CMAKE_INSTALL_LIBDIR}"
          COMPONENT YARP_carl-dev
        PUBLIC_HEADER
          DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/yarp/carl"
          COMPONENT YARP_carl-dev)

set(YARP_carl_PUBLIC_DEPS ${YARP_carl_PUBLIC_DEPS} PARENT_SCOPE)




