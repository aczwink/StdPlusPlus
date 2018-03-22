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
#ifdef _STDPLUSPLUS_BACKEND_OPENCL
//Global
#include <CL/cl.h>
//Local
#include <Std++/_Backends/ComputeBackend.hpp>
#include <Std++/Containers/Array/FixedArray.hpp>
#include "OpenCLDeviceEnumeratorState.hpp"

namespace StdPlusPlus
{
	class OpenCLBackend : public ComputeBackend
	{
	public:
		//Constructor
		inline OpenCLBackend(cl_platform_id platformId) : platformId(platformId)
		{
		}

		//Methods
		UniquePointer<_stdpp::DeviceEnumeratorState> GetDeviceEnumeratorState() const override
		{
			return new OpenCLDeviceEnumeratorState(this->platformId);
		}

		//Functions
		static FixedArray <cl_platform_id> GetPlatformIds()
		{
			cl_uint nPlatforms;
			cl_int result = clGetPlatformIDs(0, nullptr, &nPlatforms);
			ASSERT(result == CL_SUCCESS, u8"If you see this, report to Std++");

			FixedArray<cl_platform_id> platformIds(nPlatforms);
			result = clGetPlatformIDs(nPlatforms, &platformIds[0], nullptr);
			ASSERT(result == CL_SUCCESS, u8"If you see this, report to Std++");

			return platformIds;
		}

	private:
		//Members
		cl_platform_id platformId;
	};
}
#endif