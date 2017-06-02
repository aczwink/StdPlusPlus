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
//Local
#include "CL/cl.h"
#include <ACStdLib/Containers/Strings/String.hpp>
#include <ACStdLib/Containers/Strings/StringUtil.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Compute;

//TODO: DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <stdio.h> //TODO: DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <ACStdLib/Containers/Strings/ByteString.hpp>
//TODO: DELETE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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
LinkedList<Device> Compute::GetDevices()
{
	/*
	cl_int result;
	cl_uint nPlatforms, nDevices;
	cl_platform_id platformIds[10];
	cl_device_id deviceIds[10];
	LinkedList<Device> devices;

	result = clGetPlatformIDs(0, nullptr, &nPlatforms);
	ASSERT(result == CL_SUCCESS);
	ASSERT(nPlatforms < sizeof(platformIds) / sizeof(platformIds[0]));
	result = clGetPlatformIDs(nPlatforms, platformIds, nullptr);
	ASSERT(result == CL_SUCCESS);

	for(cl_int i = 0; i < nPlatforms; i++)
	{
		result = clGetDeviceIDs(platformIds[i], CL_DEVICE_TYPE_DEFAULT, 0, nullptr, &nDevices);
		ASSERT(result == CL_SUCCESS);
		ASSERT(nDevices < sizeof(deviceIds) / sizeof(deviceIds[0]));
		result = clGetDeviceIDs(platformIds[i], CL_DEVICE_TYPE_DEFAULT, nDevices, deviceIds, nullptr);
		ASSERT(result == CL_SUCCESS);

		for(cl_int j = 0; j < nDevices; j++)
		{
			Variant deviceId;

			deviceId.ptr = deviceIds[j];
			devices.InsertTail(Device(deviceId));
		}
	}

	return devices;
	 */
	NOT_IMPLEMENTED_ERROR;
}