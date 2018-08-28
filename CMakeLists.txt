cmake_minimum_required(VERSION 3.6)
project(Std++)
find_package(XP-CMake REQUIRED) #we need advanced cmake modules

#options
option(BACKENDS_AUTO_AVAILABLE "If enabled, all backends available on the host machine are enabled. If disabled, no backends but the specified ones are selected" ON)
option(BUILD_DOCUMENTATION "Build API documentation" OFF)
option(OPTFORHOST "Optimize for host machine. Output will quite likely not run on an older processor" ON)

#general settings
XPC_AutoBuildType()
if(OPTFORHOST)
	XPC_OptForHost()
endif()
XPC_SetCompileDefinitions()
#c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#---TARGET: Std++---
set(STDPLUSPLUS_PRIVATE_LIBRARIES)
add_library(Std++ SHARED)


add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(src_backends)
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	add_subdirectory(src_os/posix_shared)
	add_subdirectory(src_os/linux)
elseif(UNIX)
	add_subdirectory(src_os/posix_shared)
	add_subdirectory(src_os/posix)
elseif(WIN32)
	add_subdirectory(src_os/windows)
	target_link_libraries(Std++ PRIVATE opengl32.lib)
	target_link_libraries(Std++ PRIVATE ws2_32.lib)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) #necessary, so that all symbols are exported for windows dlls
endif()
target_sources(Std++ PRIVATE ${SOURCE_FILES})


target_include_directories(Std++ PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	)
target_link_libraries(Std++ PRIVATE ${STDPLUSPLUS_PRIVATE_LIBRARIES})

if(WIN32)
	target_compile_definitions(Std++ PRIVATE _STDPLUSPLUS_EXPORT)
endif()





#---TARGET: Std++Static---
add_library(Std++Static STATIC src_static/main.cpp src_static/main.hpp src_static/StdPlusPlusMain.cpp src_static/windows.cpp)
IF(WIN32) #windows defines operator new and delete per module (i.e. dll)
	target_sources(Std++Static PRIVATE src/MemoryOperators.cpp) #link this into the static lib
ENDIF()
target_include_directories(Std++Static PRIVATE "include")




#---TARGET: Std++Test---
add_library(Std++Test STATIC src_test/main.cpp)
target_include_directories(Std++Test PRIVATE "include")







#---TARGET: Tests---
add_subdirectory(tests)
add_executable(tests_Std++ ${TEST_FILES})

target_include_directories(tests_Std++ PUBLIC "include")
target_link_libraries(tests_Std++ LINK_PUBLIC Std++ Std++Static Std++Test)






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
install(TARGETS Std++ Std++Static EXPORT StdPlusPlusConfig
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
Install_ExportAndRegisterPackage("StdPlusPlusConfig" "StdPlusPlus")

#install public Std++
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