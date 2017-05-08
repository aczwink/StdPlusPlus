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
//Class header
#include "CShaderProgram.h"
//Local
#include "CShader.h"

//Static members
const CShaderProgram *CShaderProgram::pCurrentProgram = nullptr;

//Constructor
CShaderProgram::CShaderProgram()
{
    this->id = glCreateProgram();
}

//Destructor
CShaderProgram::~CShaderProgram()
{
    glDeleteProgram(this->id);
}

//Public methods
void CShaderProgram::AttachShader(const Shader *pShader)
{
    glAttachShader(this->id, ((CShader *)pShader)->id);
}

void CShaderProgram::DetachShader(const Shader *pShader)
{
    glDetachShader(this->id, ((CShader *)pShader)->id);
}

uint32 CShaderProgram::GetUniformId(const ByteString &refVarName) const
{
    return glGetUniformLocation(this->id, refVarName.GetC_Str());
}

void CShaderProgram::Link()
{
    glLinkProgram(this->id);
}

void CShaderProgram::SetUniformValue(uint32 varId, int32 value)
{
    this->Use();

    glUniform1i(varId, value);
}

void CShaderProgram::SetUniformValue(uint32 varId, uint32 value)
{
    this->Use();

    glUniform1ui(varId, value);
}

void CShaderProgram::SetUniformValue(uint32 varId, float32 value)
{
    this->Use();

    glUniform1f(varId, value);
}

void CShaderProgram::SetUniformValue(uint32 varId, const Math::CVector2 &refVector)
{
    this->Use();

    glUniform2fv(varId, 1, refVector.e);
}

void CShaderProgram::SetUniformValue(uint32 varId, const Vector3 &refVector)
{
    this->Use();

    glUniform3fv(varId, 1, &refVector.x);
}

void CShaderProgram::SetUniformValue(uint32 varId, const Color &refColor)
{
    this->Use();

    glUniform4fv(varId, 1, &refColor.r);
}

void CShaderProgram::SetUniformValue(uint32 varId, const CMatrix2x2 &refMatrix)
{
    this->Use();

    glUniformMatrix2fv(varId, 1, false, &refMatrix(0, 0));
}

void CShaderProgram::SetUniformValue(uint32 varId, const CMatrix3x3 &refMatrix)
{
    this->Use();

    glUniformMatrix3fv(varId, 1, false, &refMatrix(0, 0));
}

void CShaderProgram::SetUniformValue(uint32 varId, const Matrix4x4 &refMatrix)
{
    this->Use();

    glUniformMatrix4fv(varId, 1, false, &refMatrix(0, 0));
}