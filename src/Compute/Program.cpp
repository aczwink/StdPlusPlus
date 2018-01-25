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
#include <ACStdLib/Compute/Program.hpp>
//Global
#include <CL/cl.h>
//Local
#include <ACStdLib/Containers/Strings/String.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Compute;
//Definitions
#define THIS ((cl_program)this->internal)

//Constructor
Program::Program(void *internal)
{
	this->internal = internal;

	clBuildProgram(THIS, 0, nullptr, nullptr, nullptr, nullptr);
}

//Destructor
Program::~Program()
{
	clReleaseProgram(THIS);
}

//Public methods
Kernel Program::GetKernel(const ByteString &kernelName)
{
	cl_kernel kernel = clCreateKernel(THIS, kernelName.GetC_Str(), nullptr);

	ASSERT(kernel, u8"The kernel '" + String(kernelName.GetC_Str()) + u8"' does not exist.");

	return Kernel(kernel);
}