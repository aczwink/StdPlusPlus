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
#include "Containers/Map/Map.hpp"
#include "Containers/Strings/OldString.hpp"
#include "Containers/Strings/StringUtil.h"
#include "Definitions.h"
#include "Filesystem/Path.hpp"
#include "Streams/SeekableInputStream.hpp"

namespace StdXX
{
    /**
     * A configuration file with sections and key-value pairs like INI or many other cfg files.
     */
    class STDPLUSPLUS_API ConfigurationFile
    {
    public:
        //Constructor
        ConfigurationFile(const Path &refPath, bool readOnly = false);
        //Destructor
        ~ConfigurationFile();

        //Inline
        inline bool ContainsSection(const String &refSectionName)
        {
            return this->sections.Contains(refSectionName);
        }

        inline bool ContainsValue(const String &refSectionName, const String &refKey) const
        {
            return this->sections.Contains(refSectionName) && this->sections[refSectionName].Contains(refKey);
        }

        inline bool GetBoolValue(const String &refSectionName, const String &refKey) const
        {
            return this->GetStringValue(refSectionName, refKey) == u8"true";
        }

        inline int64 GetIntValue(const String &refSectionName, const String &refKey) const
        {
            return this->GetStringValue(refSectionName, refKey).ToInt();
        }

        inline String GetStringValue(const String &refSectionName, const String &refKey) const
        {
            return this->sections[refSectionName][refKey];
        }

        inline void SetValue(const String &refSectionName, const String &refKey, bool value)
        {
            this->sections[refSectionName][refKey] = value ? u8"true" : u8"false";
        }

        inline void SetValue(const String &refSectionName, const String &refKey, const String &refValue)
        {
            this->sections[refSectionName][refKey] = refValue;
        }

        inline void SetValue(const String &refSectionName, const String &refKey, int64 value)
        {
            this->sections[refSectionName][refKey] = String::Number(value);
        }

	private:
		//Members
		bool readOnly;
		Path path;
		Map<String, Map<String, String>> sections;
		//Methods
		String ReadKey(InputStream &refInput);
		String ReadSectionTitle(InputStream &refInput);
		void ReadSectionValues(const String &refSectionName, SeekableInputStream &refInput);
		String ReadValue(InputStream &refInput);
    };
}