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
		${CMAKE_CURRENT_SOURCE_DIR}/OpenGLBackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/OpenGLBackend.hpp

		PARENT_SCOPE)

	target_compile_definitions(Std++ PRIVATE _STDPLUSPLUS_BACKEND_OPENGL)
	if(UNIX)
		#here we need GLX
		set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${GLX_LIBRARY} PARENT_SCOPE)
	endif(UNIX)
endif ()