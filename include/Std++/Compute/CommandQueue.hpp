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
#include "DeviceContext.hpp"
#include "Kernel.hpp"

namespace StdPlusPlus
{
	namespace Compute
	{
		class CommandQueue
		{
		public:
			//Constructor
			CommandQueue(const DeviceContext &dc);

			//Destructor
			~CommandQueue();

			//Methods
			void EnqueueReadBuffer(const Buffer &buffer, uint32 offset, uint32 size, void *destination);
			void EnqueueTask(const Kernel &kernel);
			/**
			 * Makes sure that all queued commands have been issued and completed.
			 */
			void Finish();
			/**
			 * Makes sure that all queued commands have been issued to the device.
			 * Does not guarantee that commands have finished execution.
			 */
			void Flush();

		private:
			//Members
			void *internal;
		};
	}
}