/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Filesystem/UnixPermissions.hpp>
//Namespaces
using namespace StdXX::Filesystem;

//Constructor
UnixPermissions::UnixPermissions(uint32 encodedPermissions)
{
	UnixPermission* permissionsToSet[] = {&this->others, &this->group, &this->owner};
	for(UnixPermission*& permission : permissionsToSet)
	{
		permission->execute = (encodedPermissions & 1u) != 0;
		permission->write = (encodedPermissions & 2u) != 0;
		permission->read = (encodedPermissions & 4u) != 0;
		encodedPermissions >>= 3u;
	}
}

//Public methods
uint32 UnixPermissions::Encode() const
{
	uint32 encoded = 0;
	const UnixPermission* permissionsToSet[] = {&this->owner, &this->group, &this->others};
	for(const UnixPermission*& permission : permissionsToSet)
	{
		encoded <<= 3u;
		if(permission->read)
			encoded |= 4u;
		if(permission->write)
			encoded |= 2u;
		if(permission->execute)
			encoded |= 1u;
	}
	return encoded;
}
