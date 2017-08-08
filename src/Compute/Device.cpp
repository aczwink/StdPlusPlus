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
#include <ACStdLib/Compute/Device.hpp>
//Global
#include <CL/cl.h>
//Local
#include <ACStdLib/Memory.h>
#include <ACStdLib/Containers/Strings/String.hpp>
#include <ACStdLib/Containers/Strings/StringUtil.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Compute;

//Local functions
static cl_platform_id ChoosePlatform()
{
	cl_uint nPlatforms;
	cl_int result = clGetPlatformIDs(0, nullptr, &nPlatforms);
	ASSERT(result == CL_SUCCESS);

	cl_platform_id platformIds[10];
	ASSERT(nPlatforms < sizeof(platformIds) / sizeof(platformIds[0]));
	result = clGetPlatformIDs(nPlatforms, platformIds, nullptr);
	ASSERT(result == CL_SUCCESS);

	//TODO: currently we just return the first one
	ASSERT(nPlatforms == 1);
	return platformIds[0];
}

//Public methods
String Device::GetName() const
{
	cl_int result;
	size_t size;
	char buffer[1024];

	result = clGetDeviceInfo((cl_device_id)this->deviceId.ptr, CL_DEVICE_NAME, sizeof(buffer), buffer, &size);
	ASSERT(result == CL_SUCCESS);

	return String(buffer);
}

//Compute functions
DynamicArray<Device *> Compute::QueryDevices()
{
	cl_platform_id platformId = ChoosePlatform();
	cl_uint nDevices;
	cl_int result = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_DEFAULT, 0, nullptr, &nDevices);
	ASSERT(result == CL_SUCCESS);

	cl_device_id deviceIds[10];
	ASSERT(nDevices < sizeof(deviceIds) / sizeof(deviceIds[0]));
	result = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_DEFAULT, nDevices, deviceIds, nullptr);
	ASSERT(result == CL_SUCCESS);

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
	ASSERT(devices.GetNumberOfElements() == 1);

	return devices[0];
}