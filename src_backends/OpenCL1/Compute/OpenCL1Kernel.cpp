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
#include "OpenCL1Kernel.hpp"
//Local
#include "OpenCL1Buffer.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Compute;

//Destructor
OpenCL1Kernel::~OpenCL1Kernel()
{
	clReleaseKernel(this->kernel);
}

//Public methods
void OpenCL1Kernel::SetArg(uint8 argIndex, Buffer &buffer)
{
	OpenCL1Buffer &cl1Buffer = dynamic_cast<OpenCL1Buffer &>(buffer);
	clSetKernelArg(this->kernel, argIndex, sizeof(cl_mem), &cl1Buffer.GetCLObject());
}