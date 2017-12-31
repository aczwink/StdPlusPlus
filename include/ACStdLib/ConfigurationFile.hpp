/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
//Local
#include "Containers/Map/Map.hpp"
#include "ACStdLib/Containers/Strings/OldString.hpp"
#include "ACStdLib/Containers/Strings/StringUtil.h"
#include "Definitions.h"
#include "ACStdLib/Filesystem/Path.hpp"
#include "ACStdLib/Streams/SeekableInputStream.hpp"

namespace ACStdLib
{
    /**
     * A configuration file with sections and key-value pairs like INI or many other cfg files.
     */
    class ACSTDLIB_API ConfigurationFile
    {
    public:
        //Constructor
        ConfigurationFile(const Path &refPath, bool readOnly = false);
        //Destructor
        ~ConfigurationFile();

        //Inline
        inline bool ContainsSection(const OldString &refSectionName)
        {
            return this->sections.Contains(refSectionName);
        }

        inline bool ContainsValue(const OldString &refSectionName, const OldString &refKey) const
        {
            return this->sections.Contains(refSectionName) && this->sections[refSectionName].Contains(refKey);
        }

        inline bool GetBoolValue(const OldString &refSectionName, const OldString &refKey) const
        {
            return this->GetStringValue(refSectionName, refKey) == "true";
        }

        inline int32 GetIntValue(const OldString &refSectionName, const OldString &refKey) const
        {
            return (int32)StringToInt64(this->GetStringValue(refSectionName, refKey));
        }

        inline OldString GetStringValue(const OldString &refSectionName, const OldString &refKey) const
        {
            return this->sections[refSectionName][refKey];
        }

        inline void SetValue(const OldString &refSectionName, const OldString &refKey, bool value)
        {
            this->sections[refSectionName][refKey] = value ? "true" : "false";
        }

        inline void SetValue(const OldString &refSectionName, const OldString &refKey, const OldString &refValue)
        {
            this->sections[refSectionName][refKey] = refValue;
        }

        inline void SetValue(const OldString &refSectionName, const OldString &refKey, int64 value)
        {
            this->sections[refSectionName][refKey] = ToString(value);
        }

	private:
		//Members
		bool readOnly;
		Path path;
		Map<OldString, Map<OldString, OldString>> sections;
		//Methods
		OldString ReadKey(InputStream &refInput);
		OldString ReadSectionTitle(InputStream &refInput);
		void ReadSectionValues(const OldString &refSectionName, SeekableInputStream &refInput);
		OldString ReadValue(InputStream &refInput);
    };
}