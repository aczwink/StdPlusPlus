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


void LoadOpenGLExtensions(void *(*pLoadFunction)(const char *pExtension))
{
    glActiveTexture = (PFNGLACTIVETEXTUREPROC)pLoadFunction("glActiveTexture");
    glAttachShader = (PFNGLATTACHSHADERPROC)pLoadFunction("glAttachShader");

    glBindBuffer = (PFNGLBINDBUFFERPROC)pLoadFunction("glBindBuffer");
    glBindFramebuffer = (PFNGLBINDBUFFERPROC)pLoadFunction("glBindFramebuffer");
    glBindTexture = (PFNGLBINDTEXTUREPROC)pLoadFunction("glBindTexture");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)pLoadFunction("glBindVertexArray");
    glBlendFunc = (PFNGLBLENDFUNCPROC)pLoadFunction("glBlendFunc");
    glBufferData = (PFNGLBUFFERDATAPROC)pLoadFunction("glBufferData");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)pLoadFunction("glBufferSubData");

    glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)pLoadFunction("glCheckFramebufferStatus");
    glClear = (PFNGLCLEARPROC)pLoadFunction("glClear");
    glClearColor = (PFNGLCLEARCOLORPROC)pLoadFunction("glClearColor");
    glColorMask = (PFNGLCOLORMASKPROC)pLoadFunction("glColorMask");
    glCompileShader = (PFNGLCOMPILESHADERPROC)pLoadFunction("glCompileShader");
    glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)pLoadFunction("glCompressedTexImage2D");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)pLoadFunction("glCreateProgram");
    glCreateShader = (PFNGLCREATESHADERPROC)pLoadFunction("glCreateShader");

    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)pLoadFunction("glDeleteBuffers");
    glDeleteFramebuffers = (PFNGLDELETEBUFFERSPROC)pLoadFunction("glDeleteFramebuffers");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)pLoadFunction("glDeleteProgram");
    glDeleteShader = (PFNGLDELETESHADERPROC)pLoadFunction("glDeleteShader");
    glDeleteTextures = (PFNGLDELETETEXTURESPROC)pLoadFunction("glDeleteTextures");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)pLoadFunction("glDeleteVertexArrays");
    glDepthFunc = (PFNGLDEPTHFUNCPROC)pLoadFunction("glDepthFunc");
    glDepthMask = (PFNGLDEPTHMASKPROC)pLoadFunction("glDepthMask");
    glDetachShader = (PFNGLDETACHSHADERPROC)pLoadFunction("glDetachShader");
    glDisable = (PFNGLDISABLEPROC)pLoadFunction("glDisable");
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)pLoadFunction("glDisableVertexAttribArray");
    glDrawArrays = (PFNGLDRAWARRAYSPROC)pLoadFunction("glDrawArrays");
    glDrawBuffer = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)pLoadFunction("glDrawBuffer");
    glDrawElements = (PFNGLDRAWELEMENTSPROC)pLoadFunction("glDrawElements");

    glEnable = (PFNGLENABLEVERTEXATTRIBARRAYPROC)pLoadFunction("glEnable");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)pLoadFunction("glEnableVertexAttribArray");

    glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)pLoadFunction("glFramebufferTexture2D");

    glGenBuffers = (PFNGLGENBUFFERSPROC)pLoadFunction("glGenBuffers");
    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)pLoadFunction("glGenerateMipmap");
    glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)pLoadFunction("glGenFramebuffers");
    glGenTextures = (PFNGLGENVERTEXARRAYSPROC)pLoadFunction("glGenTextures");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)pLoadFunction("glGenVertexArrays");
    glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)pLoadFunction("glGetAttribLocation");
    glGetError = (PFNGLGETERRORPROC)pLoadFunction("glGetError");
    glGetIntegerv = (PFNGLGETINTEGERVPROC)pLoadFunction("glGetIntegerv");
    glGetProgramiv = (PFNGLGETSHADERIVPROC)pLoadFunction("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETSHADERINFOLOGPROC)pLoadFunction("glGetProgramInfoLog");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)pLoadFunction("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)pLoadFunction("glGetShaderInfoLog");
    glGetTexParameteriv = (PFNGLGETTEXPARAMETERIVPROC)pLoadFunction("glGetTexParameteriv");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)pLoadFunction("glGetUniformLocation");

    glLinkProgram = (PFNGLLINKPROGRAMPROC)pLoadFunction("glLinkProgram");

    glPolygonMode = (PFNGLPOLYGONMODEPROC)pLoadFunction("glPolygonMode");
	glPointSize = (PFNGLPOINTSIZEPROC)pLoadFunction("glPointSize");

    glReadBuffer = (PFNGLREADBUFFERPROC)pLoadFunction("glReadBuffer");

    glShaderSource = (PFNGLSHADERSOURCEPROC)pLoadFunction("glShaderSource");
    glStencilFunc = (PFNGLSTENCILFUNCPROC)pLoadFunction("glStencilFunc");
    glStencilMask = (PFNGLSTENCILMASKPROC)pLoadFunction("glStencilMask");
    glStencilOp = (PFNGLSTENCILOPPROC)pLoadFunction("glStencilOp");
    glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)pLoadFunction("glStencilOpSeparate");

    glTexImage2D = (PFNGLTEXIMAGE2DPROC)pLoadFunction("glTexImage2D");
    glTexParameteri = (PFNGLTEXPARAMETERIPROC)pLoadFunction("glTexParameteri");
    glTexSubImage2D = (PFNGLTEXIMAGE2DPROC)pLoadFunction("glTexSubImage2D");

    glUseProgram = (PFNGLUSEPROGRAMPROC)pLoadFunction("glUseProgram");
    glUniform1f = (PFNGLUNIFORM1FPROC)pLoadFunction("glUniform1f");
    glUniform1i = (PFNGLUNIFORM1IPROC)pLoadFunction("glUniform1i");
    glUniform1ui = (PFNGLUNIFORM1UIPROC)pLoadFunction("glUniform1ui");
    glUniform2fv = (PFNGLUNIFORM2FVPROC)pLoadFunction("glUniform2fv");
    glUniform3fv = (PFNGLUNIFORM3FVPROC)pLoadFunction("glUniform3fv");
    glUniform4fv = (PFNGLUNIFORM4FVPROC)pLoadFunction("glUniform4fv");
    glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)pLoadFunction("glUniformMatrix2fv");
    glUniformMatrix3fv = (PFNGLUNIFORMMATRIX4FVPROC)pLoadFunction("glUniformMatrix3fv");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)pLoadFunction("glUniformMatrix4fv");

    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)pLoadFunction("glVertexAttribPointer");
    glViewport = (PFNGLVIEWPORTPROC)pLoadFunction("glViewport");
}