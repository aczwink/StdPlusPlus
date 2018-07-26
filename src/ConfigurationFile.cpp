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
//Class Header
#include <Std++/ConfigurationFile.hpp>
//Local
#include <Std++/Streams/FileInputStream.hpp>
#include <Std++/Streams/FileOutputStream.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
//Namespaces
using namespace StdXX;

//Constructor
ConfigurationFile::ConfigurationFile(const Path &refPath, bool readOnly)
{
	NOT_IMPLEMENTED_ERROR; //TODO: new file system implementation
	//if(!refPath.Exists())
	{
		FileOutputStream file(refPath);
	}

	FileInputStream input(refPath);

	this->readOnly = readOnly;
	this->path = refPath;

	//throw CException(SJCLIBE_STREAMNOTOPEN, "File \"" + filename + "\" couldn't be opened.");

	while(true)
	{
		if(input.IsAtEnd())
			break;

		NOT_IMPLEMENTED_ERROR; //TODO: reimplement next line
		//if(input.ReadByte() == u8'[')
		{
			String name;

			name = this->ReadSectionTitle(input);
			this->ReadSectionValues(name, input);
		}
	}
}

//Destructor
ConfigurationFile::~ConfigurationFile()
{
	if(!this->readOnly)
	{
		FileOutputStream outFile(this->path);

		for(const auto &refSection : this->sections)
		{
			outFile << '[' << refSection.key << ']' << endl;

			for(const auto &refKV : refSection.value)
			{
				outFile << refKV.key << '=' << refKV.value << endl;
			}
			outFile << endl;
		}
	}
}

//Private methods
String ConfigurationFile::ReadKey(InputStream &refInput)
{
	char c;
	String key;

	while(true)
	{
		if(refInput.IsAtEnd())
			break;

		refInput.ReadBytes(&c, 1);
		if(c == '=')
			return key;
		key += c;
	}
	//throw CException(SJCLIBE_REACHEDEOF, "Expected '=' after a key");
	NOT_IMPLEMENTED_ERROR;
}

String ConfigurationFile::ReadSectionTitle(InputStream &refInput)
{
	char c;
	String section;

	DataReader reader(false, refInput);
	while(true)
	{
		if(refInput.IsAtEnd())
			break;

		c = reader.ReadByte();
		if(c == ']')
		{
			while(reader.ReadByte() != '\n'); //Read until next line

			return section;
		}

		section += c;
	}

	//throw CException(SJCLIBE_REACHEDEOF, "Unclosed sectionname.");
	NOT_IMPLEMENTED_ERROR;
}

void ConfigurationFile::ReadSectionValues(const String &refSectionName, SeekableInputStream &refInput)
{
	char c;
	String key;

	while(true)
	{
		if(refInput.IsAtEnd())
			break;

		refInput.ReadBytes(&c, 1);
		if(c == '\n')
			break;
		if(c == '\r') //Ignore
			continue;

		refInput.Rewind(1);
		key = this->ReadKey(refInput);
		this->sections[refSectionName][key] = this->ReadValue(refInput);
	}
}

String ConfigurationFile::ReadValue(InputStream &refInput)
{
	char c;
	String value;

	while(true)
	{
		if(refInput.IsAtEnd())
			break;

		refInput.ReadBytes(&c, 1);
		if(c == '\n')
			break;
		if(c == '\r') //ignore
			continue;
		value += c;
	}

	return value;
}