option(BACKEND_OPENGL "OpenGL Render-backend" OFF)


if(${BACKENDS_AUTO_AVAILABLE})
	#check for x11
	find_package(X11)
	if(X11_FOUND)
		find_package(PkgConfig)
		if (PkgConfig_FOUND)
			pkg_check_modules(PC_GLX QUIET glx)
			if(${PC_GLX_FOUND})
				set(BACKEND_OPENGL ON)
			endif()
		endif()
	endif()

	#apple has opengl
	if(APPLE)
		set(BACKEND_OPENGL ON)
	endif()
endif ()




if(${BACKEND_OPENGL})
	set(SOURCE_FILES
		${SOURCE_FILES}

		#Rendering
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLCubeMap.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLCubeMap.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLDeviceContext.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLDeviceContext.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLFrameBuffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLFrameBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLIndexBuffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLIndexBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLInputState.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLInputState.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLShader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLShader.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLShaderProgram.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLShaderProgram.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLTexture2D.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLTexture2D.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLVertexBuffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLVertexBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/Shared.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/Shared.hpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/GL2Functions.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/GL3Functions.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Import.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/LoadWindowSystemOpenGLExtension.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/OpenGL3CoreBackend.hpp
		)

	target_compile_definitions(Std++ PRIVATE _STDPLUSPLUS_BACKEND_OPENGL)

	if(MSVC)
		#MSVC does not deliver GL headers
		target_include_directories(Std++ PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/_extheaders")
	endif()

	#Cocoa is needed on macOS for getting opengl extensions
	if(APPLE)
		target_compile_definitions(Std++ PRIVATE _STDPP_WINDOWSYSTEM_COCOA)
	endif()

	#GLX is needed for getting opengl extensions on X11
	find_package(X11)
	if(X11_FOUND)
		find_package(PkgConfig REQUIRED)
		pkg_check_modules(PC_GLX QUIET glx)
		find_library(GLX_LIBRARY
			GL
			HINTS ${PC_GLX_LIBDIR} ${PC_GLX_LIBRARY_DIRS}
			)
		set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${GLX_LIBRARY})
		target_compile_definitions(Std++ PRIVATE _STDPP_WINDOWSYSTEM_X11)
	endif()

	#Windows
	if(WIN32)
		target_compile_definitions(Std++ PRIVATE _STDPP_WINDOWSYSTEM_WINDOWS)
	endif()

	#set in parent scope
	set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)
endif ()
set(BACKEND_OPENGL ${BACKEND_OPENGL} PARENT_SCOPE)