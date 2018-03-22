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
#include <GL/glcorearb.h>
#include <GL/glext.h>

//Data-exchange
extern PFNGLPIXELSTOREIPROC glPixelStorei;

//Frame buffer
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;

//Render buffer
extern PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
extern PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
extern PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
extern PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;

//Function pointers
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLBINDTEXTUREPROC glBindTexture;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLBLENDFUNCPROC glBlendFunc;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
extern PFNGLCLEARPROC glClear;
extern PFNGLCLEARCOLORPROC glClearColor;
extern PFNGLCOLORMASKPROC glColorMask;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLDELETETEXTURESPROC glDeleteTextures;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLDEPTHFUNCPROC glDepthFunc;
extern PFNGLDEPTHMASKPROC glDepthMask;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLDISABLEPROC glDisable;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLDRAWARRAYSPROC glDrawArrays;
extern PFNGLDRAWBUFFERPROC glDrawBuffer;
extern PFNGLDRAWELEMENTSPROC glDrawElements;
extern PFNGLENABLEPROC glEnable;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLGENTEXTURESPROC glGenTextures;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLGETERRORPROC glGetError;
extern PFNGLGETINTEGERVPROC glGetIntegerv;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGETTEXPARAMETERIVPROC glGetTexParameteriv;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLPOLYGONMODEPROC glPolygonMode;
extern PFNGLPOINTSIZEPROC glPointSize;
extern PFNGLREADBUFFERPROC glReadBuffer;
extern PFNGLREADPIXELSPROC glReadPixels;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLSTENCILFUNCPROC glStencilFunc;
extern PFNGLSTENCILMASKPROC glStencilMask;
extern PFNGLSTENCILOPPROC glStencilOp;
extern PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate;
extern PFNGLTEXIMAGE2DPROC glTexImage2D;
extern PFNGLTEXPARAMETERIPROC glTexParameteri;
extern PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLVIEWPORTPROC glViewport;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM1UIPROC glUniform1ui;
extern PFNGLUNIFORM2FVPROC glUniform2fv;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORM4FVPROC glUniform4fv;
extern PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;