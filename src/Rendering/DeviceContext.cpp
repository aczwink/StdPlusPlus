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
#include <Std++/Rendering/DeviceContext.hpp>
//Local
#include "../../src_backends/OpenGL3Core/Rendering/OpenGLCubeMap.hpp"
#include "../../src_backends/OpenGL3Core/Rendering/OpenGLFrameBuffer.hpp"
#include "../../src_backends/OpenGL3Core/Rendering/OpenGLIndexBuffer.h"
#include "../../src_backends/OpenGL3Core/Rendering/OpenGLShaderProgram.hpp"
#include "../../src_backends/OpenGL3Core/Rendering/OpenGLTexture2D.hpp"
#include "../../src_backends/OpenGL3Core/Rendering/OpenGLVertexBuffer.hpp"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Rendering;
using namespace StdPlusPlus::UI;

//Global variables
uint32 g_currentTextureId = Natural<uint32>::Max();