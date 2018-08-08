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
#pragma once
//Local
#include "Kernel.hpp"

namespace StdXX
{
	namespace Compute
	{
		class CommandQueue
		{
		public:
			//Destructor
			virtual ~CommandQueue() {}

			//Abstract
			virtual void EnqueueReadBuffer(const Buffer &buffer, bool block, uint32 offset, uint32 size, void *destination) = 0;
			virtual void EnqueueTask(const Kernel &kernel, uint32 globalWorkSize) = 0;
			virtual void EnqueueWriteBuffer(Buffer &buffer, bool block, uint32 offset, uint32 size, const void *src) = 0;
			/**
			* Makes sure that all queued commands have been issued and completed.
			*/
			virtual void Finish() = 0;

			//Methods
			/**
			 * Makes sure that all queued commands have been issued to the device.
			 * Does not guarantee that commands have finished execution.
			 */
			void Flush();
		};
	}
}