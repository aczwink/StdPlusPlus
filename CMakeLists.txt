cmake_minimum_required(VERSION 3.6)
project(ACStdLib)

set(CMAKE_CXX_STANDARD 11)

#debug or release
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D_DEBUG)

    set(CMAKE_DEBUG_POSTFIX "_d")
else()
    message(FATAL_ERROR "Unknown build type. Must be either 'Debug' or 'Release'")
endif()

#dependancies
if(UNIX)
    #gtk3 is required
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    include_directories(${GTK3_INCLUDE_DIRS})
    add_definitions(${GTK3_CFLAGS_OTHER})
endif(UNIX)

#targets
add_subdirectory(src)

if (UNIX)
    add_subdirectory(src_os/linux)
endif (UNIX)

add_library(ACStdLib SHARED ${SOURCE_FILES})
add_library(ACStdLibStatic STATIC src_static/main.cpp src_static/main.cpp src_static/main.h src_static/ACMain.cpp src_static/windows.cpp)

if(UNIX)
    target_link_libraries(ACStdLib ${GTK3_LIBRARIES})
endif(UNIX)


#---INSTALLATION---

#install target
install(TARGETS ACStdLib ACStdLibStatic EXPORT ACStdLibConfig
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
install(EXPORT ACStdLibConfig DESTINATION share/ACStdLib/cmake)

#install public headers
function(installHeaderDir sourceDir targetDir)
    file(GLOB LOCAL_FILES_AND_DIRS "${sourceDir}/*")
    foreach(ITEM ${LOCAL_FILES_AND_DIRS})
        IF(IS_DIRECTORY "${ITEM}")
            LIST(APPEND LOCAL_DIRS "${ITEM}")
        ELSE()
            LIST(APPEND LOCAL_FILES "${ITEM}")
        ENDIF()
    endforeach()

    INSTALL(DIRECTORY ${LOCAL_DIRS} DESTINATION "include/${targetDir}")
    INSTALL(FILES ${LOCAL_FILES} DESTINATION include/${targetDir})
endfunction()

installHeaderDir("${PROJECT_SOURCE_DIR}/headers" "ACStdLib")
installHeaderDir("${PROJECT_SOURCE_DIR}/headers_cpu" "ACStdLibCPUOpt")

#install include files
install(FILES include/ACStdLib.hpp include/ACStdLibCPUOpt.hpp DESTINATION include)