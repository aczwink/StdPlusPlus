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
#include <Std++/Devices/ComputeDevice.hpp>
#include "Compute/OpenCL1DeviceContext.hpp"
//Namespaces
using namespace StdXX;

class OpenCLDevice : public ComputeDevice
{
public:
	//Constructor
	inline OpenCLDevice(cl_device_id deviceId) : deviceId(deviceId)
	{
	}

	//Methods
	Compute::DeviceContext *CreateDeviceContext() const override
	{
		return new _stdxx_::OpenCL1DeviceContext(this->deviceId);
	}

	String GetName() const override
	{
		char buffer[4096];
		cl_int result = clGetDeviceInfo(this->deviceId, CL_DEVICE_NAME, sizeof(buffer), buffer, nullptr);
		ASSERT(result == CL_SUCCESS, u8"If you see this, report to Std++");

		return String::CopyRawString(buffer);
	}

private:
	//Members
	cl_device_id deviceId;
};