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
#include "OpenGLShader.hpp"
//Local
#include <Std++/Debug.hpp>
#include <Std++/Memory.h>
#include <Std++/Containers/Array/FixedArray.hpp>
#include "../GL3Functions.hpp"

//Constructor
OpenGLShader::OpenGLShader(Shader::ShaderType type, OpenGLDeviceContext &deviceContext) : deviceContext(deviceContext), glFuncs(deviceContext.glFuncs)
{
    switch(type)
    {
        case ShaderType::FragmentShader:
            this->id = this->glFuncs.glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case ShaderType::GeometryShader:
            this->id = this->glFuncs.glCreateShader(GL_GEOMETRY_SHADER);
            break;
        case ShaderType::VertexShader:
            this->id = this->glFuncs.glCreateShader(GL_VERTEX_SHADER);
            break;
        default:
            NOT_IMPLEMENTED_ERROR;
    }
}

//Destructor
OpenGLShader::~OpenGLShader()
{
    this->glFuncs.glDeleteShader(this->id);
}

//Public methods
bool OpenGLShader::Compile(SeekableInputStream &refSource)
{
    byte *pShaderText;
    int32 result;

    //read shader code
    pShaderText = (byte *)MemAlloc((uint32)refSource.GetRemainingBytes() + 1);
    pShaderText[refSource.GetRemainingBytes()] = 0;
    refSource.ReadBytes(pShaderText, (uint32)refSource.GetRemainingBytes());

    //compile shader
    this->glFuncs.glShaderSource(this->id, 1, (const char * const *)&pShaderText, nullptr);
    this->glFuncs.glCompileShader(this->id);
    this->glFuncs.glGetShaderiv(this->id, GL_COMPILE_STATUS, &result);

    //clean up
    MemFree(pShaderText);

    return result != 0;
}

String OpenGLShader::GetCompilationLog()
{
    GLint len;
    this->glFuncs.glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &len);
    FixedArray<char> msg(static_cast<uint32>(len - 1));
    this->glFuncs.glGetShaderInfoLog(this->id, len, nullptr, &msg[0]);

	return String::CopyRawString(&msg[0]);
}