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
//Global
#ifdef XPC_OS_DARWIN
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif
//Local
#include <Std++/Compute/CommandQueue.hpp>

namespace _stdxx_
{
	class OpenCL1CommandQueue : public StdXX::Compute::CommandQueue
	{
	public:
		//Constructor
		inline OpenCL1CommandQueue(cl_command_queue cmdQueue) : cmdQueue(cmdQueue)
		{
		}

		//Destructor
		~OpenCL1CommandQueue();

		//Methods
		void EnqueueReadBuffer(const StdXX::Compute::Buffer &buffer, bool block, uint32 offset, uint32 size, void *destination) override;
		void EnqueueTask(const StdXX::Compute::Kernel &kernel, uint32 globalWorkSize) override;
		void EnqueueWriteBuffer(StdXX::Compute::Buffer &buffer, bool block, uint32 offset, uint32 size, const void *src) override;
		void Finish() override;

	private:
		//Members
		cl_command_queue cmdQueue;
	};
}