/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/Streams/OutputStream.hpp>
#include <Std++/Variant.hpp>

namespace _stdxx_
{
	class TCPSocketOutputStream : public StdXX::OutputStream
	{
	public:
		//Constructor
		inline TCPSocketOutputStream(const StdXX::Variant &socketSystemHandle) : socketSystemHandle(socketSystemHandle)
		{
		}

		//Destructor
		~TCPSocketOutputStream();

		//Methods
		void Flush() override;
		uint32 WriteBytes(const void *source, uint32 size) override;

	private:
		//Members
		StdXX::Variant socketSystemHandle;
	};
}