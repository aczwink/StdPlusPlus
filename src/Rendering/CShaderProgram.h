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
#include <ACStdLib/Rendering/ShaderProgram.hpp>
#include "OpenGL.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACStdLib::Rendering;

class CShaderProgram : public ShaderProgram
{
private:
    //Members
    uint32 id;

    //Static members
    static const CShaderProgram *pCurrentProgram;

public:
    //Constructor
    CShaderProgram();

    //Destructor
    ~CShaderProgram();

    //Methods
    void AttachShader(const Shader *pShader);
    void DetachShader(const Shader *pShader);
    uint32 GetUniformId(const ACStdLib::ByteString &refVarName) const;
    void Link();
    void SetUniformValue(uint32 varId, int32 value);
    void SetUniformValue(uint32 varId, uint32 value);
    void SetUniformValue(uint32 varId, float32 value);
    void SetUniformValue(uint32 varId, const Vector2 &refVector);
    void SetUniformValue(uint32 varId, const Vector3 &refVector);
    void SetUniformValue(uint32 varId, const Color &refColor);
    void SetUniformValue(uint32 varId, const Matrix2x2 &refMatrix);
    void SetUniformValue(uint32 varId, const CMatrix3x3 &refMatrix);
    void SetUniformValue(uint32 varId, const Matrix4x4 &refMatrix);

    //Inline
    inline void Use() const
    {
        if(this != CShaderProgram::pCurrentProgram)
        {
            glUseProgram(this->id);
            CShaderProgram::pCurrentProgram = this;
        }
    }
};