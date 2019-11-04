add_subdirectory(libavcodec)
add_subdirectory(liblzma)
add_subdirectory(OpenALSoft)
add_subdirectory(OpenCL1)
add_subdirectory(OpenGL3Core)
add_subdirectory(OpenSSL)
add_subdirectory(XAudio2)
add_subdirectory(zlib)

add_subdirectory(Cocoa)
add_subdirectory(CommCtrl)
add_subdirectory(gtk3)

set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)

set(SOURCE_FILES
	${SOURCE_FILES}

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/RegisterBackends.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RegisterExtensions.cpp

	PARENT_SCOPE)