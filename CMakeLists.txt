cmake_minimum_required(VERSION 3.6)
project(ACStdLib)

#options
option(USE_LIBAVCODEC "Use libavcodec so that additional codecs can be used. YOU SHOULD INFORM YOURSELF ABOUT LICENSE ISSUES BEFORE ENABLING THIS!" OFF)
if(USE_LIBAVCODEC)
	add_definitions(-D_AC_LIB_USEAVCODEC)
	include(cmake/FindFFmpeg.cmake)
endif(USE_LIBAVCODEC)


option(BUILD_DOCUMENTATION "Build documentation" OFF)

#c++ standard
set(CMAKE_CXX_STANDARD 14) #TODO: #13 - make this 17
set(CMAKE_CXX_STANDARD_REQUIRED ON) #we need C++17 because of template class type deduction, else C++11 would be sufficient

#currently SSE4.1 required. However, only 2 should be required for x64
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")

#debug or release
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D_DEBUG)

    set(CMAKE_DEBUG_POSTFIX "_d")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
	#ok
else()
    message(FATAL_ERROR "Unknown build type. Must be either 'Debug' or 'Release'")
endif()

#dependancies
find_package(OpenCL REQUIRED)
if(UNIX)
    #gtk3 is required
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    include_directories(${GTK3_INCLUDE_DIRS})
    add_definitions(${GTK3_CFLAGS_OTHER})

	pkg_check_modules(PC_GLX QUIET glx)
	find_library(GLX_LIBRARY
		GL
		HINTS ${PC_GLX_LIBDIR} ${PC_GLX_LIBRARY_DIRS}
		)
endif(UNIX)

#collect source files
add_subdirectory(include/ACStdLib)
add_subdirectory(src)
add_subdirectory(tests)

if (UNIX)
    add_subdirectory(src_os/linux)
endif (UNIX)
if(WIN32)
	add_subdirectory(src_os/windows)
endif(WIN32)

add_library(ACStdLib SHARED ${SOURCE_FILES})
add_library(ACStdLibStatic STATIC src_static/main.cpp src_static/main.cpp src_static/main.h src_static/ACMain.cpp src_static/windows.cpp)
add_library(ACStdLibTest STATIC src_test/main.cpp)
add_executable(tests_ACStdLib ${TEST_FILES})

#platform stuff
if(WIN32)
	set_target_properties(ACStdLib PROPERTIES COMPILE_DEFINITIONS _ACSTDLIB_EXPORT)
endif()

#includes
target_include_directories(ACStdLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(ACStdLibTest PUBLIC "include")

target_include_directories(ACStdLib PRIVATE "ext")
target_include_directories(ACStdLib PRIVATE ${OpenCL_INCLUDE_DIRS})

#link
target_link_libraries(ACStdLib PRIVATE ${OpenCL_LIBRARIES})
if(UNIX)
    target_link_libraries(ACStdLib PRIVATE ${GTK3_LIBRARIES})
	target_link_libraries(ACStdLib PRIVATE ${GLX_LIBRARY})
endif(UNIX)
if(WIN32)
	target_link_libraries(ACStdLib PRIVATE opengl32.lib)
endif(WIN32)

if(USE_LIBAVCODEC)
	target_link_libraries(ACStdLib PRIVATE ${FFMPEG_LIBRARIES})
endif(USE_LIBAVCODEC)

target_include_directories(tests_ACStdLib PUBLIC "include")
target_link_libraries(tests_ACStdLib LINK_PUBLIC ACStdLib ACStdLibStatic ACStdLibTest)



#---DOCUMENTATION---
if(BUILD_DOCUMENTATION)
	find_package(Doxygen REQUIRED)
	if(NOT DOXYGEN_FOUND)
		message(FATAL_ERROR "Doxygen is needed to build the documentation.")
	endif()

	set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
	set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

	configure_file(${doxyfile_in} ${doxyfile} @ONLY)

	add_custom_target(doc
		COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation"
		VERBATIM)

	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
endif()




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