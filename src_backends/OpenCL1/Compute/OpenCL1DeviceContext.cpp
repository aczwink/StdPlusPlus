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
#include "OpenCL1DeviceContext.hpp"
//Local
#include "OpenCL1Buffer.hpp"
#include "OpenCL1CommandQueue.hpp"
#include "OpenCL1Program.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Compute;

//Destructor
OpenCL1DeviceContext::~OpenCL1DeviceContext()
{
	clReleaseContext(this->context);
}

//Public methods
Buffer *OpenCL1DeviceContext::CreateBuffer(uint32 size, bool read, bool write)
{
	cl_mem_flags memFlag;

	ASSERT(read || write, u8"A buffer must be either readable or writeable (or both).");
	if (read && write)
		memFlag = CL_MEM_READ_WRITE;
	else if (read)
		memFlag = CL_MEM_READ_ONLY;
	else if (write)
		memFlag = CL_MEM_WRITE_ONLY;

	cl_mem memObj = clCreateBuffer(this->context, memFlag, size, nullptr, nullptr);

	return new OpenCL1Buffer(memObj);
}

CommandQueue *OpenCL1DeviceContext::CreateCommandQueue()
{
	cl_device_id deviceId;
	clGetContextInfo(this->context, CL_CONTEXT_DEVICES, sizeof(deviceId), &deviceId, nullptr);

	cl_command_queue queue = clCreateCommandQueue(this->context, deviceId, 0, nullptr);

	return new OpenCL1CommandQueue(queue);
}

Program * OpenCL1DeviceContext::CreateProgram(const String & source)
{
	const char *src = (const char *)source.ToUTF8().GetRawZeroTerminatedData();
	size_t len = source.GetLength();
	cl_program prog = clCreateProgramWithSource(this->context, 1, &src, &len, nullptr);

	return new OpenCL1Program(prog);
}