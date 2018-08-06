/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include "Import.hpp"

struct GLFunctions_2_0
{
	//Buffers
	PFNGLBINDBUFFERPROC glBindBuffer;
	PFNGLBUFFERDATAPROC glBufferData;
	PFNGLBUFFERSUBDATAPROC glBufferSubData;
	PFNGLDELETEBUFFERSPROC glDeleteBuffers;
	PFNGLGENBUFFERSPROC glGenBuffers;
	PFNGLREADBUFFERPROC glReadBuffer;

	//Data-exchange
	PFNGLPIXELSTOREIPROC glPixelStorei;

	//Multimedia
	PFNGLBLENDFUNCPROC glBlendFunc;
	PFNGLCLEARPROC glClear;
	PFNGLCLEARCOLORPROC glClearColor;
	PFNGLCOLORMASKPROC glColorMask;
	PFNGLDEPTHFUNCPROC glDepthFunc;
	PFNGLDEPTHMASKPROC glDepthMask;
	PFNGLDISABLEPROC glDisable;
	PFNGLDRAWARRAYSPROC glDrawArrays;
	PFNGLDRAWBUFFERPROC glDrawBuffer;
	PFNGLDRAWELEMENTSPROC glDrawElements;
	PFNGLENABLEPROC glEnable;
	PFNGLGETERRORPROC glGetError;
	PFNGLGETINTEGERVPROC glGetIntegerv;
	PFNGLPOLYGONMODEPROC glPolygonMode;
	PFNGLPOINTSIZEPROC glPointSize;
	PFNGLREADPIXELSPROC glReadPixels;
	PFNGLVIEWPORTPROC glViewport;

	//Shaders
	PFNGLATTACHSHADERPROC glAttachShader;
	PFNGLCOMPILESHADERPROC glCompileShader;
	PFNGLCREATEPROGRAMPROC glCreateProgram;
	PFNGLCREATESHADERPROC glCreateShader;
	PFNGLDELETEPROGRAMPROC glDeleteProgram;
	PFNGLDELETESHADERPROC glDeleteShader;
	PFNGLDETACHSHADERPROC glDetachShader;
	PFNGLGETPROGRAMIVPROC glGetProgramiv;
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
	PFNGLGETSHADERIVPROC glGetShaderiv;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	PFNGLLINKPROGRAMPROC glLinkProgram;
	PFNGLSHADERSOURCEPROC glShaderSource;
	PFNGLUSEPROGRAMPROC glUseProgram;
	PFNGLUNIFORM1FPROC glUniform1f;
	PFNGLUNIFORM1IPROC glUniform1i;
	PFNGLUNIFORM2FVPROC glUniform2fv;
	PFNGLUNIFORM3FVPROC glUniform3fv;
	PFNGLUNIFORM4FVPROC glUniform4fv;
	PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
	PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

	//Stencil
	PFNGLSTENCILFUNCPROC glStencilFunc;
	PFNGLSTENCILMASKPROC glStencilMask;
	PFNGLSTENCILOPPROC glStencilOp;
	PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate;

	//Texturing
	PFNGLACTIVETEXTUREPROC glActiveTexture;
	PFNGLBINDTEXTUREPROC glBindTexture;
	PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
	PFNGLDELETETEXTURESPROC glDeleteTextures;
	PFNGLGENTEXTURESPROC glGenTextures;
	PFNGLGETTEXPARAMETERIVPROC glGetTexParameteriv;
	PFNGLTEXIMAGE2DPROC glTexImage2D;
	PFNGLTEXPARAMETERIPROC glTexParameteri;
	PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D;

	//Vertex Attributes
	PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
};