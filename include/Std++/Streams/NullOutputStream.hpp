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
#pragma once
//Local
#include "OutputStream.hpp"

namespace StdXX
{
	/**
	 * A dummy output stream that does not write anywhere.
	 * The stream accepts any input and reports successful writes.
	 * The write method is essentially a no-op.
	 * It does not write to /dev/null or anything like that.
	 *
	 * This class can be useful when for instance wanting to hash a file.
	 * The FileInputStream can be piped into the HashingInputStream which is then connected to a NullOutputStream.
	 * This way data is read but not written anywhere without the need to extend functionality of the processing input stream (e.g. HashingInputStream).
	 */
	class NullOutputStream : public OutputStream
	{
	public:
		//Methods
		void Flush() override;
		uint32 WriteBytes(const void *source, uint32 size) override;
	};
}