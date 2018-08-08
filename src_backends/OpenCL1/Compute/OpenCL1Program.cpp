/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of Std++.
 *
 * Std++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Std++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Std++.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include "OpenCL1Program.hpp"
//Local
#include "OpenCL1Kernel.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Compute;

//Destructor
OpenCL1Program::~OpenCL1Program()
{
	clReleaseProgram(this->program);
}

//Public methods
bool OpenCL1Program::Build()
{
	cl_int result = clBuildProgram(this->program, 0, nullptr, nullptr, nullptr, nullptr);
	switch (result)
	{
	case CL_SUCCESS:
		return true;
	case CL_BUILD_PROGRAM_FAILURE:
		return false;
	default:
		ASSERT(result == CL_SUCCESS, u8"TODO: implement error handling");
	}

	return false;
}

Kernel * OpenCL1Program::CreateKernel(const String &kernelName)
{
	cl_int errorCode;
	cl_kernel kernel = clCreateKernel(this->program, (const char *)kernelName.ToUTF8().GetRawZeroTerminatedData(), &errorCode);
	switch (errorCode)
	{
	case CL_SUCCESS:
		break;
	case CL_INVALID_KERNEL_NAME:
		ASSERT(kernel, u8"The kernel '" + kernelName + u8"' does not exist.");
	default:
		NOT_IMPLEMENTED_ERROR;
	}

	return new OpenCL1Kernel(kernel);
}

String OpenCL1Program::GetBuildLog() const
{
	//get device
	cl_device_id deviceId;
	clGetProgramInfo(this->program, CL_PROGRAM_DEVICES, sizeof(deviceId), &deviceId, nullptr);

	char buffer[4096];
	size_t copiedSize;
	clGetProgramBuildInfo(this->program, deviceId, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &copiedSize);
	
	return String::CopyRawString(buffer);
}