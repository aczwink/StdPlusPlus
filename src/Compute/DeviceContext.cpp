/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <ACStdLib/Compute/DeviceContext.hpp>
//Global
#include <CL/cl.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Compute;
//Definitions
#define THIS ((cl_context)this->internal)

//Constructor
DeviceContext::DeviceContext(const Device &device)
{
	this->internal = clCreateContext(nullptr, 1, reinterpret_cast<cl_device_id const *>(&device.deviceId.ptr), nullptr, nullptr, nullptr);
}

//Destructor
DeviceContext::~DeviceContext()
{
	clReleaseContext(THIS);
}

//Public methods
Buffer DeviceContext::CreateBuffer(uint32 size, bool read, bool write)
{
	cl_mem_flags memFlag;

	if(read && write)
		memFlag = CL_MEM_READ_WRITE;
	else if(read)
		memFlag = CL_MEM_READ_ONLY;
	else if(write)
		memFlag = CL_MEM_WRITE_ONLY;
	else
	{
		ERROR("A buffer must be either readable or writeable (or both).");
	}

	cl_mem memObj = clCreateBuffer(THIS, memFlag, size, nullptr, nullptr);

	return Buffer(memObj);
}

Program DeviceContext::CreateProgram(const ByteString &source)
{
	const char *src = source.GetC_Str();
	size_t len = source.GetLength();
	cl_program prog = clCreateProgramWithSource(THIS, 1, &src, &len, nullptr);

	return Program(prog);
}