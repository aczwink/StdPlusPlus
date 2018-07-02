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
#include <Std++/_Backends/DeviceEnumeratorState.hpp>
#include <Std++/Containers/Array/FixedArray.hpp>
#include "OpenCLDevice.hpp"
//Namespaces
using namespace StdPlusPlus;

static cl_uint GetNumberOfDevices(cl_platform_id platformId)
{
	cl_uint nDevices;
	cl_int result = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_DEFAULT, 0, nullptr, &nDevices);
	ASSERT(result == CL_SUCCESS, u8"If you see this, report to Std++");

	return nDevices;
}

class OpenCLDeviceEnumeratorState : public _stdpp::DeviceEnumeratorState
{
public:
	//Constructor
	inline OpenCLDeviceEnumeratorState(cl_platform_id platformId) : deviceIds(GetNumberOfDevices(platformId)), currentIndex(0)
	{
		cl_int result = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_DEFAULT, this->deviceIds.GetNumberOfElements(), &this->deviceIds[0], nullptr);
		ASSERT(result == CL_SUCCESS, u8"If you see this, report to Std++");
	}

	//Methods
	AutoPointer<Device> GetNextDevice()
	{
		if(this->currentIndex >= this->deviceIds.GetNumberOfElements())
			return nullptr;

		return new OpenCLDevice(this->deviceIds[this->currentIndex++]);
	}

private:
	//Members
	FixedArray<cl_device_id> deviceIds;
	uint32 currentIndex;
};