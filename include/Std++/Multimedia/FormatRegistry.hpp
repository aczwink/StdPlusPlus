/*
* Copyright (c) 2017-2018,2021-2022 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/ShutdownManager.hpp>
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include "CodingFormat.hpp"
#include "Format.hpp"

namespace StdXX::Multimedia
{
    class FormatRegistry : public Releasable
    {
    public:
        //Properties
        inline const DynamicArray<UniquePointer<Format>>& ContainerFormats() const
        {
            return this->formats;
        }

        //Methods
        const CodingFormat* FindCodingFormatById(CodingFormatId codingFormatId);
        void Register(Decoder* decoder, float32 quality);
        void Release() override;

        //Functions
        inline static const CodingFormat* GetCodingFormatById(CodingFormatId codingFormatId)
        {
            return Instance().FindCodingFormatById(codingFormatId);
        }

        inline static FormatRegistry& Instance()
        {
            static FormatRegistry instance;
            return instance;
        }

        //Inline
        inline void Register(UniquePointer<CodingFormat>&& codingFormat)
        {
            this->codingFormats.Insert(codingFormat->GetId(), Move(codingFormat));
        }

        inline void Register(UniquePointer<Format>&& format)
        {
            this->formats.Push(Move(format));
        }

    private:
        //Members
        BinaryTreeMap<CodingFormatId, UniquePointer<CodingFormat>> codingFormats;
        DynamicArray<UniquePointer<Format>> formats;

        //Constructor
        FormatRegistry();

        //Methods
        void RegisterCodingFormats();
        void RegisterContainerFormats();
        void RegisterDecoders();
    };
}