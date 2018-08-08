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
#include "OpenCL1CommandQueue.hpp"
//Local
#include "OpenCL1Buffer.hpp"
#include "OpenCL1Kernel.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Compute;

//Destructor
OpenCL1CommandQueue::~OpenCL1CommandQueue()
{
	clReleaseCommandQueue(this->cmdQueue);
}

//Public methods
void OpenCL1CommandQueue::EnqueueReadBuffer(const Buffer &buffer, bool block, uint32 offset, uint32 size, void *destination)
{
	const OpenCL1Buffer &cl1Buffer = dynamic_cast<const OpenCL1Buffer &>(buffer);
	clEnqueueReadBuffer(this->cmdQueue, cl1Buffer.GetCLObject(), block ? CL_TRUE : CL_FALSE, offset, size, destination, 0, nullptr, nullptr);
}

void OpenCL1CommandQueue::EnqueueTask(const Kernel &kernel, uint32 globalWorkSize)
{
	const OpenCL1Kernel &cl1Kernel = dynamic_cast<const OpenCL1Kernel &>(kernel);
	size_t global = globalWorkSize;
	clEnqueueNDRangeKernel(this->cmdQueue, cl1Kernel.GetCLObject(), 1, nullptr, &global, nullptr, 0, nullptr, nullptr);
}

void OpenCL1CommandQueue::EnqueueWriteBuffer(Buffer &buffer, bool block, uint32 offset, uint32 size, const void *src)
{
	OpenCL1Buffer &cl1Buffer = dynamic_cast<OpenCL1Buffer &>(buffer);
	clEnqueueWriteBuffer(this->cmdQueue, cl1Buffer.GetCLObject(), block ? CL_TRUE : CL_FALSE, offset, size, src, 0, nullptr, nullptr);
}

void OpenCL1CommandQueue::Finish()
{
	clFinish(this->cmdQueue);
}

/*
void CommandQueue::Flush()
{
	clFlush(THIS);
}*/