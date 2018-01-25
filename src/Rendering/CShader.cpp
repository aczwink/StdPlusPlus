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
#include "CShader.h"
//Local
#include <ACStdLib/Debug.h>
#include <ACStdLib/Memory.h>
#include "OpenGL.h"

//Constructor
CShader::CShader(Shader::ShaderType type)
{
    switch(type)
    {
        case ShaderType::FragmentShader:
            this->id = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case ShaderType::GeometryShader:
            this->id = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        case ShaderType::VertexShader:
            this->id = glCreateShader(GL_VERTEX_SHADER);
            break;
        default:
            NOT_IMPLEMENTED_ERROR;
    }
}

//Destructor
CShader::~CShader()
{
    glDeleteShader(this->id);
}

//Public methods
bool CShader::Compile(SeekableInputStream &refSource)
{
    byte *pShaderText;
    int32 result;

    //read shader code
    pShaderText = (byte *)MemAlloc((uint32)refSource.GetRemainingBytes() + 1);
    pShaderText[refSource.GetRemainingBytes()] = 0;
    refSource.ReadBytes(pShaderText, (uint32)refSource.GetRemainingBytes());

    //compile shader
    glShaderSource(this->id, 1, (const char * const *)&pShaderText, nullptr);
    glCompileShader(this->id);
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &result);

    //clean up
    MemFree(pShaderText);

    return result != 0;
}

ByteString CShader::GetCompilationLog()
{
    GLint len;
    ByteString result;

    glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &len);
    result.Resize(len - 1);
    glGetShaderInfoLog(this->id, len, NULL, (char *)result.GetC_Str());

    return result;
}