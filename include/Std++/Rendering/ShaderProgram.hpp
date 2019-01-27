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
#include "../Color.hpp"
#include "../Containers/Strings/ByteString.hpp"
#include "../Math/Matrix2x2.hpp"
#include "../Math/Matrix3x3.hpp"
#include "../Math/Matrix4x4.hpp"
#include "Std++/Math/Vector2/Vector2.hpp"
#include "../Math/Vector3.hpp"

namespace StdXX
{
    namespace Rendering
    {
        //Move declarations
        class Shader;

        class STDPLUSPLUS_API ShaderProgram
        {
        public:
            //Destructor
            virtual ~ShaderProgram() {}

            //Abstract
            virtual void AttachShader(const Shader *pShader) = 0;
            virtual void DetachShader(const Shader *pShader) = 0;
            virtual uint32 GetUniformId(const StdXX::ByteString &refVarName) const = 0;
            virtual void Link() = 0;
            virtual void SetUniformValue(uint32 varId, int32 value) = 0;
            virtual void SetUniformValue(uint32 varId, uint32 value) = 0;
            virtual void SetUniformValue(uint32 varId, float32 value) = 0;
            virtual void SetUniformValue(uint32 varId, const Math::Vector2S &refVector) = 0;
            virtual void SetUniformValue(uint32 varId, const Math::Vector3S &refVector) = 0;
            virtual void SetUniformValue(uint32 varId, const Color &refColor) = 0;
            virtual void SetUniformValue(uint32 varId, const Math::Matrix2s &refMatrix) = 0;
            virtual void SetUniformValue(uint32 varId, const Math::Matrix3S &refMatrix) = 0;
            virtual void SetUniformValue(uint32 varId, const Math::Matrix4S &refMatrix) = 0;
        };
    }
}