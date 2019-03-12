set(SOURCE_FILES
	${SOURCE_FILES}

	#VectorPath
	${CMAKE_CURRENT_SOURCE_DIR}/VectorPath/DevicePathRenderer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/VectorPath/FlatVectorPath.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/VectorPath/HostComputedPathRenderer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/VectorPath/VectorPath.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/VectorPath/VectorPathRenderer.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/DeviceContext.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/IndexBuffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/InputLayout.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/InputState.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Shader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ShaderProgram.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Texture2D.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/VertexBuffer.hpp

	PARENT_SCOPE)