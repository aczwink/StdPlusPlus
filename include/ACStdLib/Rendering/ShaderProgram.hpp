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
#include "ACStdLib/Color.h"
#include "ACStdLib/Containers/Strings/ByteString.hpp"
#include "../Math/Geometry/CMatrix2x2.h"
#include "../Math/Geometry/CMatrix3x3.h"
#include "ACStdLib/Math/Geometry/Matrix4x4.hpp"
#include "../Math/Geometry/CVector2.h"
#include "ACStdLib/Math/Geometry/Vector3.hpp"

namespace ACStdLib
{
    namespace Rendering
    {
        //Forward declarations
        class Shader;

        class ACSTDLIB_API ShaderProgram
        {
        public:
            //Destructor
            virtual ~ShaderProgram() {}

            //Abstract
            virtual void AttachShader(const Shader *pShader) = NULL;
            virtual void DetachShader(const Shader *pShader) = NULL;
            virtual uint32 GetUniformId(const ACStdLib::ByteString &refVarName) const = NULL;
            virtual void Link() = NULL;
            virtual void SetUniformValue(uint32 varId, int32 value) = NULL;
            virtual void SetUniformValue(uint32 varId, uint32 value) = NULL;
            virtual void SetUniformValue(uint32 varId, float32 value) = NULL;
            virtual void SetUniformValue(uint32 varId, const Math::CVector2 &refVector) = NULL;
            virtual void SetUniformValue(uint32 varId, const Math::Vector3 &refVector) = NULL;
            virtual void SetUniformValue(uint32 varId, const Color &refColor) = NULL;
            virtual void SetUniformValue(uint32 varId, const Math::CMatrix2x2 &refMatrix) = NULL;
            virtual void SetUniformValue(uint32 varId, const Math::CMatrix3x3 &refMatrix) = NULL;
            virtual void SetUniformValue(uint32 varId, const Math::Matrix4x4 &refMatrix) = NULL;
        };
    }
}