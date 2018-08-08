/*
* Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Compute/DeviceContext.hpp>

namespace _stdxx_
{
	class OpenCL1DeviceContext : public StdXX::Compute::DeviceContext
	{
	public:
		//Constructor
		inline OpenCL1DeviceContext(cl_device_id deviceId)
		{
			this->context = clCreateContext(nullptr, 1, &deviceId, nullptr, nullptr, nullptr);
		}

		//Destructor
		~OpenCL1DeviceContext();

		//Methods
		StdXX::Compute::Buffer * CreateBuffer(uint32 size, bool read, bool write) override;
		StdXX::Compute::CommandQueue * CreateCommandQueue() override;
		StdXX::Compute::Program * CreateProgram(const StdXX::String & source) override;

	private:
		//Members
		cl_context context;
	};
}