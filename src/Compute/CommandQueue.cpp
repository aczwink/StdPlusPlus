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
#include <ACStdLib/Compute/CommandQueue.hpp>
//Global
#include <CL/cl.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Compute;
//Definitions
#define THIS ((cl_command_queue)this->internal)

//Constructor
CommandQueue::CommandQueue(const DeviceContext &dc)
{
	cl_context ctx = static_cast<cl_context>(dc.internal);
	cl_device_id deviceId;
	clGetContextInfo(ctx, CL_CONTEXT_DEVICES, sizeof(deviceId), &deviceId, nullptr);

	this->internal = clCreateCommandQueueWithProperties(ctx, deviceId, nullptr, nullptr);
}

//Destructor
CommandQueue::~CommandQueue()
{
	clReleaseCommandQueue(THIS);
}

//Public methods
void CommandQueue::EnqueueReadBuffer(const Buffer &buffer, uint32 offset, uint32 size, void *destination)
{
	clEnqueueReadBuffer(THIS, static_cast<cl_mem>(buffer.internal), CL_FALSE, offset, size, destination, 0, nullptr, nullptr);
}

void CommandQueue::EnqueueTask(const Kernel &kernel)
{
	size_t global = 1, local = 1;

	clEnqueueNDRangeKernel(THIS, static_cast<cl_kernel>(kernel.internal), 1, nullptr, &global, &local, 0, nullptr, nullptr);
}

void CommandQueue::Finish()
{
	clFinish(THIS);
}

void CommandQueue::Flush()
{
	clFlush(THIS);
}