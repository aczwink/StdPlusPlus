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
namespace _stdxx_
{
	union WaitObjHandle
	{
		void *handle;
		int fd;
	};
}

namespace StdXX
{
	class EventWaiter
	{
	public:
		//Constructors
		/**
		 * Constructs new waiter.
		 */
		EventWaiter();
		/**
		 * Shallow copies handle. It will not be freed when this is destructed.
		 */
		EventWaiter(_stdxx_::WaitObjHandle handle);

		//Destructor
		~EventWaiter();

		//Methods
		void Signal();

		//Inline
		inline _stdxx_::WaitObjHandle& GetNativeHandle()
		{
			return this->nativeHandle;
		}

	private:
		//Members
		bool owned;
		_stdxx_::WaitObjHandle nativeHandle;
	};
}