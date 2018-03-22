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
//Class header
#include "OpenGLBackend.hpp"
//Local
#include "GLFunctions.h"
//Namespaces
using namespace StdPlusPlus;

//Constructor
OpenGLBackend::OpenGLBackend(GL_EXT_LOADER extensionLoaderFunction) : extensionsLoaded(false),
																	  extensionLoaderFunction(extensionLoaderFunction)
{
}

//Public methods
void OpenGLBackend::Load()
{
	if(this->extensionsLoaded)
		return;

	this->LoadOpenGLExtensions();

	this->extensionsLoaded = true;
}

//Private methods
void OpenGLBackend::LoadOpenGLExtensions()
{
	//Data-exchange
	glPixelStorei = (PFNGLPIXELSTOREIPROC)this->extensionLoaderFunction("glPixelStorei");

	//Frame buffer
	glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)this->extensionLoaderFunction("glFramebufferRenderbuffer");

	//Render buffer
	glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)this->extensionLoaderFunction("glBindRenderbuffer");
	glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)this->extensionLoaderFunction("glDeleteRenderbuffers");
	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)this->extensionLoaderFunction("glGenRenderbuffers");
	glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)this->extensionLoaderFunction("glRenderbufferStorage");

	//Sorted by name:
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)this->extensionLoaderFunction("glActiveTexture");
	glAttachShader = (PFNGLATTACHSHADERPROC)this->extensionLoaderFunction("glAttachShader");

	glBindBuffer = (PFNGLBINDBUFFERPROC)this->extensionLoaderFunction("glBindBuffer");
	glBindFramebuffer = (PFNGLBINDBUFFERPROC)this->extensionLoaderFunction("glBindFramebuffer");
	glBindTexture = (PFNGLBINDTEXTUREPROC)this->extensionLoaderFunction("glBindTexture");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)this->extensionLoaderFunction("glBindVertexArray");
	glBlendFunc = (PFNGLBLENDFUNCPROC)this->extensionLoaderFunction("glBlendFunc");
	glBufferData = (PFNGLBUFFERDATAPROC)this->extensionLoaderFunction("glBufferData");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)this->extensionLoaderFunction("glBufferSubData");

	glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)this->extensionLoaderFunction("glCheckFramebufferStatus");
	glClear = (PFNGLCLEARPROC)this->extensionLoaderFunction("glClear");
	glClearColor = (PFNGLCLEARCOLORPROC)this->extensionLoaderFunction("glClearColor");
	glColorMask = (PFNGLCOLORMASKPROC)this->extensionLoaderFunction("glColorMask");
	glCompileShader = (PFNGLCOMPILESHADERPROC)this->extensionLoaderFunction("glCompileShader");
	glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)this->extensionLoaderFunction("glCompressedTexImage2D");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)this->extensionLoaderFunction("glCreateProgram");
	glCreateShader = (PFNGLCREATESHADERPROC)this->extensionLoaderFunction("glCreateShader");

	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)this->extensionLoaderFunction("glDeleteBuffers");
	glDeleteFramebuffers = (PFNGLDELETEBUFFERSPROC)this->extensionLoaderFunction("glDeleteFramebuffers");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)this->extensionLoaderFunction("glDeleteProgram");
	glDeleteShader = (PFNGLDELETESHADERPROC)this->extensionLoaderFunction("glDeleteShader");
	glDeleteTextures = (PFNGLDELETETEXTURESPROC)this->extensionLoaderFunction("glDeleteTextures");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)this->extensionLoaderFunction("glDeleteVertexArrays");
	glDepthFunc = (PFNGLDEPTHFUNCPROC)this->extensionLoaderFunction("glDepthFunc");
	glDepthMask = (PFNGLDEPTHMASKPROC)this->extensionLoaderFunction("glDepthMask");
	glDetachShader = (PFNGLDETACHSHADERPROC)this->extensionLoaderFunction("glDetachShader");
	glDisable = (PFNGLDISABLEPROC)this->extensionLoaderFunction("glDisable");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)this->extensionLoaderFunction("glDisableVertexAttribArray");
	glDrawArrays = (PFNGLDRAWARRAYSPROC)this->extensionLoaderFunction("glDrawArrays");
	glDrawBuffer = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)this->extensionLoaderFunction("glDrawBuffer");
	glDrawElements = (PFNGLDRAWELEMENTSPROC)this->extensionLoaderFunction("glDrawElements");

	glEnable = (PFNGLENABLEVERTEXATTRIBARRAYPROC)this->extensionLoaderFunction("glEnable");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)this->extensionLoaderFunction("glEnableVertexAttribArray");

	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)this->extensionLoaderFunction("glFramebufferTexture2D");

	glGenBuffers = (PFNGLGENBUFFERSPROC)this->extensionLoaderFunction("glGenBuffers");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)this->extensionLoaderFunction("glGenerateMipmap");
	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)this->extensionLoaderFunction("glGenFramebuffers");
	glGenTextures = (PFNGLGENVERTEXARRAYSPROC)this->extensionLoaderFunction("glGenTextures");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)this->extensionLoaderFunction("glGenVertexArrays");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)this->extensionLoaderFunction("glGetAttribLocation");
	glGetError = (PFNGLGETERRORPROC)this->extensionLoaderFunction("glGetError");
	glGetIntegerv = (PFNGLGETINTEGERVPROC)this->extensionLoaderFunction("glGetIntegerv");
	glGetProgramiv = (PFNGLGETSHADERIVPROC)this->extensionLoaderFunction("glGetProgramiv");
	glGetProgramInfoLog = (PFNGLGETSHADERINFOLOGPROC)this->extensionLoaderFunction("glGetProgramInfoLog");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)this->extensionLoaderFunction("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)this->extensionLoaderFunction("glGetShaderInfoLog");
	glGetTexParameteriv = (PFNGLGETTEXPARAMETERIVPROC)this->extensionLoaderFunction("glGetTexParameteriv");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)this->extensionLoaderFunction("glGetUniformLocation");

	glLinkProgram = (PFNGLLINKPROGRAMPROC)this->extensionLoaderFunction("glLinkProgram");

	glPolygonMode = (PFNGLPOLYGONMODEPROC)this->extensionLoaderFunction("glPolygonMode");
	glPointSize = (PFNGLPOINTSIZEPROC)this->extensionLoaderFunction("glPointSize");

	glReadBuffer = (PFNGLREADBUFFERPROC)this->extensionLoaderFunction("glReadBuffer");
	glReadPixels = (PFNGLREADPIXELSPROC)this->extensionLoaderFunction("glReadPixels");

	glShaderSource = (PFNGLSHADERSOURCEPROC)this->extensionLoaderFunction("glShaderSource");
	glStencilFunc = (PFNGLSTENCILFUNCPROC)this->extensionLoaderFunction("glStencilFunc");
	glStencilMask = (PFNGLSTENCILMASKPROC)this->extensionLoaderFunction("glStencilMask");
	glStencilOp = (PFNGLSTENCILOPPROC)this->extensionLoaderFunction("glStencilOp");
	glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)this->extensionLoaderFunction("glStencilOpSeparate");

	glTexImage2D = (PFNGLTEXIMAGE2DPROC)this->extensionLoaderFunction("glTexImage2D");
	glTexParameteri = (PFNGLTEXPARAMETERIPROC)this->extensionLoaderFunction("glTexParameteri");
	glTexSubImage2D = (PFNGLTEXIMAGE2DPROC)this->extensionLoaderFunction("glTexSubImage2D");

	glUseProgram = (PFNGLUSEPROGRAMPROC)this->extensionLoaderFunction("glUseProgram");
	glUniform1f = (PFNGLUNIFORM1FPROC)this->extensionLoaderFunction("glUniform1f");
	glUniform1i = (PFNGLUNIFORM1IPROC)this->extensionLoaderFunction("glUniform1i");
	glUniform1ui = (PFNGLUNIFORM1UIPROC)this->extensionLoaderFunction("glUniform1ui");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)this->extensionLoaderFunction("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)this->extensionLoaderFunction("glUniform3fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)this->extensionLoaderFunction("glUniform4fv");
	glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)this->extensionLoaderFunction("glUniformMatrix2fv");
	glUniformMatrix3fv = (PFNGLUNIFORMMATRIX4FVPROC)this->extensionLoaderFunction("glUniformMatrix3fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)this->extensionLoaderFunction("glUniformMatrix4fv");

	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)this->extensionLoaderFunction("glVertexAttribPointer");
	glViewport = (PFNGLVIEWPORTPROC)this->extensionLoaderFunction("glViewport");
}
