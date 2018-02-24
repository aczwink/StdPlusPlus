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
#include <Std++/Compute/Device.hpp>
//Global
#include <CL/cl.h>
//Local
#include <Std++/Memory.h>
#include <Std++/Containers/Strings/OldString.hpp>
#include <Std++/Containers/Strings/StringUtil.h>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Compute;

//Local functions
static cl_platform_id ChoosePlatform()
{
	cl_uint nPlatforms;
	cl_int result = clGetPlatformIDs(0, nullptr, &nPlatforms);
	ASSERT(result == CL_SUCCESS, "If you see this, report to StdPlusPlus");

	cl_platform_id platformIds[10];
	ASSERT(nPlatforms < sizeof(platformIds) / sizeof(platformIds[0]), "If you see this, report to StdPlusPlus");
	result = clGetPlatformIDs(nPlatforms, platformIds, nullptr);
	ASSERT(result == CL_SUCCESS, "If you see this, report to StdPlusPlus");

	//TODO: currently we just return the first one
	ASSERT(nPlatforms == 1, "If you see this, report to StdPlusPlus");
	return platformIds[0];
}

//Public methods
OldString Device::GetName() const
{
	cl_int result;
	size_t size;
	char buffer[1024];

	result = clGetDeviceInfo((cl_device_id)this->deviceId.ptr, CL_DEVICE_NAME, sizeof(buffer), buffer, &size);
	ASSERT(result == CL_SUCCESS, "If you see this, report to StdPlusPlus");

	return OldString(buffer);
}

//Compute functions
DynamicArray<Device *> Compute::QueryDevices()
{
	cl_platform_id platformId = ChoosePlatform();
	cl_uint nDevices;
	cl_int result = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_DEFAULT, 0, nullptr, &nDevices);
	ASSERT(result == CL_SUCCESS, "If you see this, report to StdPlusPlus");

	cl_device_id deviceIds[10];
	ASSERT(nDevices < sizeof(deviceIds) / sizeof(deviceIds[0]), "If you see this, report to StdPlusPlus");
	result = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_DEFAULT, nDevices, deviceIds, nullptr);
	ASSERT(result == CL_SUCCESS, "If you see this, report to StdPlusPlus");

	DynamicArray<Device *> devices;
	for(cl_int i = 0; i < nDevices; i++)
	{
		Variant deviceId;

		deviceId.ptr = deviceIds[i];
		devices.Push(new Device(deviceId));
	}

	return devices;
}

Device *Compute::QueryOptimalDevice()
{
	DynamicArray<Device *> devices = QueryDevices();

	//TODO: implement this
	ASSERT(devices.GetNumberOfElements() == 1, "If you see this, report to StdPlusPlus");

	return devices[0];
}