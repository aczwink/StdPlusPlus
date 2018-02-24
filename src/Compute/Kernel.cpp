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
#include <Std++/Compute/Kernel.hpp>
//Global
#include <CL/cl.h>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Compute;
//Definitions
#define THIS ((cl_kernel)this->internal)

//Constructor
Kernel::Kernel(void *internal)
{
	this->internal = internal;
}

//Destructor
Kernel::~Kernel()
{
	clReleaseKernel(THIS);
}

//Public methods
void Kernel::SetArg(uint8 argIndex, const Buffer &buffer)
{
	clSetKernelArg(THIS, argIndex, sizeof(cl_mem), &buffer.internal);
}