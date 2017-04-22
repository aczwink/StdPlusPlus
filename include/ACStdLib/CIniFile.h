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
#include "Containers/CMap/CMap.h"
#include "Containers/Strings/CString.h"
#include "Containers/Strings/String.h"
#include "Definitions.h"
#include "Filesystem/CPath.h"
#include "Streams/ASeekableInputStream.h"

namespace ACStdLib
{
    class ACSTDLIB_API CIniFile
    {
    private:
        //Members
        bool readOnly;
        CPath path;
        CMap<CString, CMap<CString, CString>> sections;
        //Methods
        CString ReadKey(AInputStream &refInput);
        CString ReadSectionTitle(AInputStream &refInput);
        void ReadSectionValues(const CString &refSectionName, ASeekableInputStream &refInput);
        CString ReadValue(AInputStream &refInput);
    public:
        //Constructor
        CIniFile(const CPath &refPath, bool readOnly = false);
        //Destructor
        ~CIniFile();

        //Inline
        inline bool ContainsSection(const CString &refSectionName)
        {
            return this->sections.Contains(refSectionName);
        }

        inline bool ContainsValue(const CString &refSectionName, const CString &refKey) const
        {
            return this->sections.Contains(refSectionName) && this->sections[refSectionName].Contains(refKey);
        }

        inline bool GetBoolValue(const CString &refSectionName, const CString &refKey) const
        {
            return this->GetStringValue(refSectionName, refKey) == "true";
        }

        inline int32 GetIntValue(const CString &refSectionName, const CString &refKey) const
        {
            return (int32)StringToInt64(this->GetStringValue(refSectionName, refKey));
        }

        inline CString GetStringValue(const CString &refSectionName, const CString &refKey) const
        {
            return this->sections[refSectionName][refKey];
        }

        inline void SetValue(const CString &refSectionName, const CString &refKey, bool value)
        {
            this->sections[refSectionName][refKey] = value ? "true" : "false";
        }

        inline void SetValue(const CString &refSectionName, const CString &refKey, const CString &refValue)
        {
            this->sections[refSectionName][refKey] = refValue;
        }

        inline void SetValue(const CString &refSectionName, const CString &refKey, int64 value)
        {
            this->sections[refSectionName][refKey] = ToString(value);
        }
    };
}