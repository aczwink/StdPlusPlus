/*
 * Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Math/Distributions/UniformUnsignedDistribution.hpp>
#include <Std++/Math/RandomBitGenerators/LinearCongruentialGenerator.hpp>

namespace StdXX::FileSystem
{
	class TempDirectory
	{
	public:
		//Constructors
		TempDirectory()
		{
			Math::MinStdRand rng;
			this->Init(rng);
		}

		template<typename GeneratorNatType>
		TempDirectory(Math::RandomBitGenerator<GeneratorNatType>& randomBitGenerator)
		{
			this->template Init(randomBitGenerator);
		}

		//Destructor
		~TempDirectory()
		{
			FileSystemsManager::Instance().OSFileSystem().RemoveDirectory(this->path);
		}

		//Properties
		inline const class Path& Path() const
		{
			return this->path;
		}

	private:
		//Members
		class Path path;

		//Methods
		template<typename GeneratorNatType>
		void Init(Math::RandomBitGenerator<GeneratorNatType>& randomBitGenerator)
		{
			OSFileSystem& osFileSystem = FileSystemsManager::Instance().OSFileSystem();

			Math::UniformUnsignedDistribution<uint8, GeneratorNatType> distribution(randomBitGenerator);
			Optional<Errors::CreateDirectoryError> result;
			do
			{
				String name;
				for(uint8 i = 0; i < 8; i++)
					name += String::HexNumber(distribution.Next(), 2, false);
				this->path = osFileSystem.GetTempPath() / name;
				result = osFileSystem.CreateDirectory(this->path);
				if(result.HasValue() && (*result != Errors::CreateDirectoryError::FileExists))
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}
			while(result.HasValue() && (*result == Errors::CreateDirectoryError::FileExists));
		}
	};
}