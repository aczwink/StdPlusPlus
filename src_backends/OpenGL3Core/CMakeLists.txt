option(BACKEND_OPENGL "OpenGL Render-backend" OFF)


if(${BACKENDS_AUTO_AVAILABLE})
	set(BACKEND_OPENGL ON)
	set(BACKEND_OPENGL ON PARENT_SCOPE)
endif ()




if(${BACKEND_OPENGL})
	set(SOURCE_FILES
		${SOURCE_FILES}

		#Rendering
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLCubeMap.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLFrameBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLShader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLShader.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLShaderProgram.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLTexture2D.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLDeviceContext.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLDeviceContext.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLIndexBuffer.h
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLInputState.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/OpenGLVertexBuffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Rendering/Shared.hpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/GLFunctions.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/GLFunctions.h
		${CMAKE_CURRENT_SOURCE_DIR}/LoadWindowSystemOpenGLExtension.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/OpenGL3CoreBackend.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/OpenGL3CoreBackend.hpp

		PARENT_SCOPE)

	target_compile_definitions(Std++ PRIVATE _STDPLUSPLUS_BACKEND_OPENGL)

	if(MSVC OR APPLE)
		#MSVC does not deliver GL headers
		#Apple does but incompatible ones...
		target_include_directories(Std++ PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/_extheaders")
	endif()

	#GLX is needed for getting opengl extensions on X11
	find_package(X11)
	if(X11_FOUND)
		pkg_check_modules(PC_GLX QUIET glx)
		find_library(GLX_LIBRARY
			GL
			HINTS ${PC_GLX_LIBDIR} ${PC_GLX_LIBRARY_DIRS}
			)
		set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${GLX_LIBRARY} PARENT_SCOPE)
		target_compile_definitions(Std++ PRIVATE _STDPP_WINDOWSYSTEM_X11)
	endif()
endif ()