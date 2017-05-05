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
find_library(OPENCL_LIBRARY NAMES OpenCL)
if(UNIX)
    #gtk3 is required
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    include_directories(${GTK3_INCLUDE_DIRS})
    add_definitions(${GTK3_CFLAGS_OTHER})
endif(UNIX)

#targets
add_subdirectory(include/ACStdLib)
add_subdirectory(src)
#add_subdirectory(tests)

if (UNIX)
    add_subdirectory(src_os/linux)
endif (UNIX)
if(WIN32)
	add_subdirectory(src_os/windows)
endif(WIN32)

add_library(ACStdLib SHARED ${SOURCE_FILES})
add_library(ACStdLibStatic STATIC src_static/main.cpp src_static/main.cpp src_static/main.h src_static/ACMain.cpp src_static/windows.cpp)
#add_executable(ACStdLib_tests ${TEST_FILES})

#platform stuff
if(WIN32)
	set_target_properties(ACStdLib PROPERTIES COMPILE_DEFINITIONS _ACSTDLIB_EXPORT)
endif()

#includes
target_include_directories(ACStdLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

#link
target_link_libraries(ACStdLib ${OPENCL_LIBRARY})
if(UNIX)
    target_link_libraries(ACStdLib ${GTK3_LIBRARIES})
endif(UNIX)
if(WIN32)
	target_link_libraries(ACStdLib opengl32.lib)
endif(WIN32)

#target_include_directories(ACStdLib_tests PUBLIC "include")
#target_link_libraries(ACStdLib_tests LINK_PUBLIC ACStdLib ACStdLibStatic)


#---INSTALLATION---

#install target
install(TARGETS ACStdLib ACStdLibStatic EXPORT ACStdLibConfig
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
install(EXPORT ACStdLibConfig DESTINATION share/ACStdLib/cmake)

#install public ACStdLib
function(installHeaderDir sourceDir targetDir)
    file(GLOB LOCAL_FILES_AND_DIRS "${sourceDir}/*")
    foreach(ITEM ${LOCAL_FILES_AND_DIRS})
        IF(IS_DIRECTORY "${ITEM}")
            LIST(APPEND LOCAL_DIRS "${ITEM}")
        ELSE()
            LIST(APPEND LOCAL_FILES "${ITEM}")
        ENDIF()
    endforeach()

    INSTALL(DIRECTORY ${LOCAL_DIRS} DESTINATION "${targetDir}")
    INSTALL(FILES ${LOCAL_FILES} DESTINATION "${targetDir}")
endfunction()

installHeaderDir("include" "include")