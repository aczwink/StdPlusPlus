cmake_minimum_required(VERSION 3.6)
project(ACStdLib)

#options
option(BACKENDS_AUTO_AVAILABLE "If enabled, all backends available on the compiling machine are enabled. If disabled, no backend but the specified ones are selected" ON)
option(BUILD_DOCUMENTATION "Build API documentation" OFF)



#c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) #we need C++17 because of template class type deduction, else C++11 would be sufficient



#machine
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1") #currently SSE4.1 required. However, only 2 should be required for x64




#debug or release
if(CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-D_DEBUG)

	set(CMAKE_DEBUG_POSTFIX "_d")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
	#ok
else()
	message(FATAL_ERROR "Unknown build type. Must be either 'Debug' or 'Release'")
endif()




#---TARGET: ACStdLib---
set(ACSTDLIB_PRIVATE_LIBRARIES)
add_library(ACStdLib SHARED)


add_subdirectory(include/ACStdLib)
add_subdirectory(src)
add_subdirectory(src_backends)
if (UNIX)
	add_subdirectory(src_os/linux)
endif (UNIX)
if(WIN32)
	add_subdirectory(src_os/windows)
endif(WIN32)
target_sources(ACStdLib PRIVATE ${SOURCE_FILES})


target_include_directories(ACStdLib PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	)
target_link_libraries(ACStdLib PRIVATE ${ACSTDLIB_PRIVATE_LIBRARIES})




#tests target
add_subdirectory(tests)












#options
option(USE_LIBAVCODEC "Use libavcodec so that additional codecs can be used. YOU SHOULD INFORM YOURSELF ABOUT LICENSE ISSUES BEFORE ENABLING THIS!" OFF)
if(USE_LIBAVCODEC)
	add_definitions(-D_AC_LIB_USEAVCODEC)
	include(cmake/FindFFmpeg.cmake)
endif(USE_LIBAVCODEC)


#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) #necessary, so that all symbols are exported for windows dlls

#dependencies
find_package(OpenCL REQUIRED)

add_library(ACStdLibStatic STATIC src_static/main.cpp src_static/main.cpp src_static/main.h src_static/ACMain.cpp src_static/windows.cpp)
add_library(ACStdLibTest STATIC src_test/main.cpp)
add_executable(tests_ACStdLib ${TEST_FILES})

#platform stuff
if(WIN32)
	set_target_properties(ACStdLib PROPERTIES COMPILE_DEFINITIONS _ACSTDLIB_EXPORT)
endif()

#includes
target_include_directories(ACStdLibTest PUBLIC "include")

target_include_directories(ACStdLib PRIVATE "ext")
target_include_directories(ACStdLib PRIVATE ${OpenCL_INCLUDE_DIRS})

#link
target_link_libraries(ACStdLib PRIVATE ${OpenCL_LIBRARIES})
if(UNIX)
	target_link_libraries(ACStdLib PRIVATE ${GLX_LIBRARY})
endif(UNIX)
if(WIN32)
	target_link_libraries(ACStdLib PRIVATE opengl32.lib)
	target_link_libraries(ACStdLib PRIVATE ws2_32.lib)
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