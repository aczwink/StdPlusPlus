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
//Main header
#include "OpenGL.h"

//Data-exchange
PFNGLPIXELSTOREIPROC glPixelStorei;

//Frame buffer
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;

//Render buffer
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;

//Platform-independent function pointers
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLBINDTEXTUREPROC glBindTexture;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLBLENDFUNCPROC glBlendFunc;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
PFNGLCLEARPROC glClear;
PFNGLCLEARCOLORPROC glClearColor;
PFNGLCOLORMASKPROC glColorMask;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDELETETEXTURESPROC glDeleteTextures;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLDEPTHFUNCPROC glDepthFunc;
PFNGLDEPTHMASKPROC glDepthMask;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLDISABLEPROC glDisable;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLDRAWARRAYSPROC glDrawArrays;
PFNGLDRAWBUFFERPROC glDrawBuffer;
PFNGLDRAWELEMENTSPROC glDrawElements;
PFNGLENABLEPROC glEnable;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLGENTEXTURESPROC glGenTextures;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGETERRORPROC glGetError;
PFNGLGETINTEGERVPROC glGetIntegerv;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETTEXPARAMETERIVPROC glGetTexParameteriv;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLPOLYGONMODEPROC glPolygonMode;
PFNGLPOINTSIZEPROC glPointSize;
PFNGLREADBUFFERPROC glReadBuffer;
PFNGLREADPIXELSPROC glReadPixels;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLSTENCILFUNCPROC glStencilFunc;
PFNGLSTENCILMASKPROC glStencilMask;
PFNGLSTENCILOPPROC glStencilOp;
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate;
PFNGLTEXIMAGE2DPROC glTexImage2D;
PFNGLTEXPARAMETERIPROC glTexParameteri;
PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM1UIPROC glUniform1ui;
PFNGLUNIFORM2FVPROC glUniform2fv;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLUNIFORM4FVPROC glUniform4fv;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLVIEWPORTPROC glViewport;


void LoadOpenGLExtensions(void *(*loaderFunction)(const char *pExtension))
{
	//Data-exchange
	glPixelStorei = (PFNGLPIXELSTOREIPROC)loaderFunction("glPixelStorei");

	//Frame buffer
	glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)loaderFunction("glFramebufferRenderbuffer");

	//Render buffer
	glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)loaderFunction("glBindRenderbuffer");
	glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)loaderFunction("glDeleteRenderbuffers");
	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)loaderFunction("glGenRenderbuffers");
	glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)loaderFunction("glRenderbufferStorage");

	//Sorted by name:
    glActiveTexture = (PFNGLACTIVETEXTUREPROC)loaderFunction("glActiveTexture");
    glAttachShader = (PFNGLATTACHSHADERPROC)loaderFunction("glAttachShader");

    glBindBuffer = (PFNGLBINDBUFFERPROC)loaderFunction("glBindBuffer");
    glBindFramebuffer = (PFNGLBINDBUFFERPROC)loaderFunction("glBindFramebuffer");
    glBindTexture = (PFNGLBINDTEXTUREPROC)loaderFunction("glBindTexture");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)loaderFunction("glBindVertexArray");
    glBlendFunc = (PFNGLBLENDFUNCPROC)loaderFunction("glBlendFunc");
    glBufferData = (PFNGLBUFFERDATAPROC)loaderFunction("glBufferData");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)loaderFunction("glBufferSubData");

    glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)loaderFunction("glCheckFramebufferStatus");
    glClear = (PFNGLCLEARPROC)loaderFunction("glClear");
    glClearColor = (PFNGLCLEARCOLORPROC)loaderFunction("glClearColor");
    glColorMask = (PFNGLCOLORMASKPROC)loaderFunction("glColorMask");
    glCompileShader = (PFNGLCOMPILESHADERPROC)loaderFunction("glCompileShader");
    glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)loaderFunction("glCompressedTexImage2D");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)loaderFunction("glCreateProgram");
    glCreateShader = (PFNGLCREATESHADERPROC)loaderFunction("glCreateShader");

    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)loaderFunction("glDeleteBuffers");
    glDeleteFramebuffers = (PFNGLDELETEBUFFERSPROC)loaderFunction("glDeleteFramebuffers");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)loaderFunction("glDeleteProgram");
    glDeleteShader = (PFNGLDELETESHADERPROC)loaderFunction("glDeleteShader");
    glDeleteTextures = (PFNGLDELETETEXTURESPROC)loaderFunction("glDeleteTextures");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)loaderFunction("glDeleteVertexArrays");
    glDepthFunc = (PFNGLDEPTHFUNCPROC)loaderFunction("glDepthFunc");
    glDepthMask = (PFNGLDEPTHMASKPROC)loaderFunction("glDepthMask");
    glDetachShader = (PFNGLDETACHSHADERPROC)loaderFunction("glDetachShader");
    glDisable = (PFNGLDISABLEPROC)loaderFunction("glDisable");
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)loaderFunction("glDisableVertexAttribArray");
    glDrawArrays = (PFNGLDRAWARRAYSPROC)loaderFunction("glDrawArrays");
    glDrawBuffer = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)loaderFunction("glDrawBuffer");
    glDrawElements = (PFNGLDRAWELEMENTSPROC)loaderFunction("glDrawElements");

    glEnable = (PFNGLENABLEVERTEXATTRIBARRAYPROC)loaderFunction("glEnable");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)loaderFunction("glEnableVertexAttribArray");

    glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)loaderFunction("glFramebufferTexture2D");

    glGenBuffers = (PFNGLGENBUFFERSPROC)loaderFunction("glGenBuffers");
    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)loaderFunction("glGenerateMipmap");
    glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)loaderFunction("glGenFramebuffers");
    glGenTextures = (PFNGLGENVERTEXARRAYSPROC)loaderFunction("glGenTextures");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)loaderFunction("glGenVertexArrays");
    glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)loaderFunction("glGetAttribLocation");
    glGetError = (PFNGLGETERRORPROC)loaderFunction("glGetError");
    glGetIntegerv = (PFNGLGETINTEGERVPROC)loaderFunction("glGetIntegerv");
    glGetProgramiv = (PFNGLGETSHADERIVPROC)loaderFunction("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETSHADERINFOLOGPROC)loaderFunction("glGetProgramInfoLog");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)loaderFunction("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)loaderFunction("glGetShaderInfoLog");
    glGetTexParameteriv = (PFNGLGETTEXPARAMETERIVPROC)loaderFunction("glGetTexParameteriv");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)loaderFunction("glGetUniformLocation");

    glLinkProgram = (PFNGLLINKPROGRAMPROC)loaderFunction("glLinkProgram");

    glPolygonMode = (PFNGLPOLYGONMODEPROC)loaderFunction("glPolygonMode");
	glPointSize = (PFNGLPOINTSIZEPROC)loaderFunction("glPointSize");

    glReadBuffer = (PFNGLREADBUFFERPROC)loaderFunction("glReadBuffer");
	glReadPixels = (PFNGLREADPIXELSPROC)loaderFunction("glReadPixels");

    glShaderSource = (PFNGLSHADERSOURCEPROC)loaderFunction("glShaderSource");
    glStencilFunc = (PFNGLSTENCILFUNCPROC)loaderFunction("glStencilFunc");
    glStencilMask = (PFNGLSTENCILMASKPROC)loaderFunction("glStencilMask");
    glStencilOp = (PFNGLSTENCILOPPROC)loaderFunction("glStencilOp");
    glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)loaderFunction("glStencilOpSeparate");

    glTexImage2D = (PFNGLTEXIMAGE2DPROC)loaderFunction("glTexImage2D");
    glTexParameteri = (PFNGLTEXPARAMETERIPROC)loaderFunction("glTexParameteri");
    glTexSubImage2D = (PFNGLTEXIMAGE2DPROC)loaderFunction("glTexSubImage2D");

    glUseProgram = (PFNGLUSEPROGRAMPROC)loaderFunction("glUseProgram");
    glUniform1f = (PFNGLUNIFORM1FPROC)loaderFunction("glUniform1f");
    glUniform1i = (PFNGLUNIFORM1IPROC)loaderFunction("glUniform1i");
    glUniform1ui = (PFNGLUNIFORM1UIPROC)loaderFunction("glUniform1ui");
    glUniform2fv = (PFNGLUNIFORM2FVPROC)loaderFunction("glUniform2fv");
    glUniform3fv = (PFNGLUNIFORM3FVPROC)loaderFunction("glUniform3fv");
    glUniform4fv = (PFNGLUNIFORM4FVPROC)loaderFunction("glUniform4fv");
    glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)loaderFunction("glUniformMatrix2fv");
    glUniformMatrix3fv = (PFNGLUNIFORMMATRIX4FVPROC)loaderFunction("glUniformMatrix3fv");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)loaderFunction("glUniformMatrix4fv");

    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)loaderFunction("glVertexAttribPointer");
    glViewport = (PFNGLVIEWPORTPROC)loaderFunction("glViewport");
}