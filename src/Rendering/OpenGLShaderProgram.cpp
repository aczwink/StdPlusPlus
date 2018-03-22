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
//Class header
#include "../../src_backends/OpenGL/Rendering/OpenGLShaderProgram.hpp"
//Local
#include "../../src_backends/OpenGL/Rendering/OpenGLShader.hpp"

//Static members
const OpenGLShaderProgram *OpenGLShaderProgram::pCurrentProgram = nullptr;

//Constructor
OpenGLShaderProgram::OpenGLShaderProgram()
{
    this->id = glCreateProgram();
}

//Destructor
OpenGLShaderProgram::~OpenGLShaderProgram()
{
    glDeleteProgram(this->id);
}

//Public methods
void OpenGLShaderProgram::AttachShader(const Shader *pShader)
{
    glAttachShader(this->id, ((OpenGLShader *)pShader)->id);
}

void OpenGLShaderProgram::DetachShader(const Shader *pShader)
{
    glDetachShader(this->id, ((OpenGLShader *)pShader)->id);
}

uint32 OpenGLShaderProgram::GetUniformId(const ByteString &refVarName) const
{
    return glGetUniformLocation(this->id, refVarName.GetC_Str());
}

void OpenGLShaderProgram::Link()
{
    glLinkProgram(this->id);
}

void OpenGLShaderProgram::SetUniformValue(uint32 varId, int32 value)
{
    this->Use();

    glUniform1i(varId, value);
}

void OpenGLShaderProgram::SetUniformValue(uint32 varId, uint32 value)
{
    this->Use();

    glUniform1ui(varId, value);
}

void OpenGLShaderProgram::SetUniformValue(uint32 varId, float32 value)
{
    this->Use();

    glUniform1f(varId, value);
}

void OpenGLShaderProgram::SetUniformValue(uint32 varId, const Math::Vector2s &refVector)
{
    this->Use();

    glUniform2fv(varId, 1, refVector.e);
}

void OpenGLShaderProgram::SetUniformValue(uint32 varId, const Vector3s &refVector)
{
    this->Use();

    glUniform3fv(varId, 1, &refVector.x);
}

void OpenGLShaderProgram::SetUniformValue(uint32 varId, const Color &refColor)
{
    this->Use();

    glUniform4fv(varId, 1, &refColor.r);
}

void OpenGLShaderProgram::SetUniformValue(uint32 varId, const Matrix2s &refMatrix)
{
    this->Use();

    glUniformMatrix2fv(varId, 1, false, &refMatrix(0, 0));
}

void OpenGLShaderProgram::SetUniformValue(uint32 varId, const Matrix3s &refMatrix)
{
    this->Use();

    glUniformMatrix3fv(varId, 1, false, &refMatrix(0, 0));
}

void OpenGLShaderProgram::SetUniformValue(uint32 varId, const Matrix4s &refMatrix)
{
    this->Use();

    glUniformMatrix4fv(varId, 1, false, &refMatrix(0, 0));
}