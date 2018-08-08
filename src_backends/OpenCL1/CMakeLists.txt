option(BACKEND_OPENCL "OpenCL Compute-backend" OFF)



if(${BACKENDS_AUTO_AVAILABLE})
	find_package(OpenCL)
	if (OpenCL_FOUND)
		set(BACKEND_OPENCL ON)
	endif ()
endif ()




if(${BACKEND_OPENCL})
	find_package(OpenCL REQUIRED)

	set(SOURCE_FILES
		${SOURCE_FILES}

		#Compute
		${CMAKE_CURRENT_SOURCE_DIR}/Compute/OpenCL1Buffer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Compute/OpenCL1Buffer.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Compute/OpenCL1CommandQueue.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Compute/OpenCL1CommandQueue.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Compute/OpenCL1DeviceContext.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Compute/OpenCL1DeviceContext.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Compute/OpenCL1Kernel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Compute/OpenCL1Kernel.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Compute/OpenCL1Program.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Compute/OpenCL1Program.hpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/OpenCL1Backend.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/OpenCLDevice.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/OpenCLDeviceEnumeratorState.hpp

		PARENT_SCOPE)

	target_compile_definitions(Std++ PRIVATE _STDPLUSPLUS_BACKEND_OPENCL)
	target_include_directories(Std++ PRIVATE ${OpenCL_INCLUDE_DIRS})
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${OpenCL_LIBRARIES} PARENT_SCOPE)
endif ()