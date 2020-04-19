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
#include <Std++/Cryptography/CheckedHashingInputStream.hpp>
//Namespaces
using namespace StdXX::Crypto;
//Local
#include <Std++/Errorhandling/Exceptions/VerificationFailedException.hpp>

//Public methods
uint32 CheckedHashingInputStream::GetBytesAvailable() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

bool CheckedHashingInputStream::IsAtEnd() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return false;
}

uint32 CheckedHashingInputStream::ReadBytes(void *destination, uint32 count)
{
	if(this->finished)
		return 0;

	uint32 nBytesRead = this->inputStream.ReadBytes(destination, count);
	this->hasher->Update((byte*)destination, nBytesRead);

	if(!this->finished && this->inputStream.IsAtEnd())
		this->Finish();

	return nBytesRead;
}

//Private methods
void CheckedHashingInputStream::Finish()
{
	this->hasher->Finish();
	if(this->hasher->GetDigestString().ToLowercase() != this->expectedHash)
		throw ErrorHandling::VerificationFailedException();
	this->finished = true;
}
