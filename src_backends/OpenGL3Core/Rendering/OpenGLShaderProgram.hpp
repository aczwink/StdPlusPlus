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
#include <Std++/Rendering/ShaderProgram.hpp>
#include "OpenGLDeviceContext.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::Rendering;

class OpenGLShaderProgram : public ShaderProgram
{
public:
    //Constructor
    OpenGLShaderProgram(OpenGLDeviceContext &deviceContext);

    //Destructor
    ~OpenGLShaderProgram();

    //Methods
    void AttachShader(const Shader *pShader);
    void DetachShader(const Shader *pShader);
    uint32 GetUniformId(const StdXX::ByteString &refVarName) const;
    void Link();
    void SetUniformValue(uint32 varId, int32 value);
    void SetUniformValue(uint32 varId, uint32 value);
    void SetUniformValue(uint32 varId, float32 value);
    void SetUniformValue(uint32 varId, const Vector2s &refVector);
    void SetUniformValue(uint32 varId, const Vector3s &refVector);
    void SetUniformValue(uint32 varId, const Color &refColor);
    void SetUniformValue(uint32 varId, const Matrix2s &refMatrix);
    void SetUniformValue(uint32 varId, const Matrix3s &refMatrix);
    void SetUniformValue(uint32 varId, const Matrix4s &refMatrix);

    //Inline
    inline void Use() const
    {
        if(this != OpenGLShaderProgram::pCurrentProgram)
        {
        	this->glFuncs.glUseProgram(this->id);
            OpenGLShaderProgram::pCurrentProgram = this;
        }
    }
private:
	//Members
	OpenGLDeviceContext &deviceContext;
	GLFunctions_3_0 &glFuncs;
	uint32 id;

	//Static members
	static const OpenGLShaderProgram *pCurrentProgram;
};