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
#include <Std++/FileSystem/POSIXPermissions.hpp>
//Local
#include <Std++/Debug.hpp>
//Namespaces
using namespace StdXX::FileSystem;

//Constructor
POSIXPermissions::POSIXPermissions(int32 userId, int32 groupId, uint32 encodedMode)
{
    this->userId = userId;
    this->groupId = groupId;

	POSIXPermission* permissionsToSet[] = {&this->others, &this->group, &this->owner};
	for(POSIXPermission*& permission : permissionsToSet)
	{
		permission->execute = (encodedMode & 1u) != 0;
		permission->write = (encodedMode & 2u) != 0;
		permission->read = (encodedMode & 4u) != 0;
        encodedMode >>= 3u;
	}

	this->sticky = (encodedMode & 1u) != 0;
	this->setGroupIdOnExecution = (encodedMode & 2u) != 0;
    this->setUserIdOnExecution = (encodedMode & 4u) != 0;

    //remaining bits in "encodedMode" should be the file type and these are ignored here
}

//Public methods
uint32 POSIXPermissions::EncodeMode() const
{
	uint32 encoded = 0;

	if(this->setUserIdOnExecution)
        encoded |= 4u;
    if(this->setGroupIdOnExecution)
        encoded |= 2u;
    if(this->sticky)
        encoded |= 1u;

	const POSIXPermission* permissionsToSet[] = {&this->owner, &this->group, &this->others};
	for(const POSIXPermission*& permission : permissionsToSet)
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
