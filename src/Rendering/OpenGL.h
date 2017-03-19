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
#include "../../headers/Definitions.h"

typedef float GLfloat;
typedef char GLchar;
typedef unsigned char GLboolean;
typedef int GLsizei;
typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef int GLint;
typedef int64 GLsizeiptr;
typedef unsigned int GLbitfield;

//Definitions
#define GL_NONE 0
#define GL_ZERO 0
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_DEPTH_BUFFER_BIT 0x100
#define GL_LESS 0x0201
#define GL_LEQUAL 0x0203
#define GL_NOTEQUAL 0x205
#define GL_ALWAYS 0x207
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_FRONT 0x404
#define GL_BACK 0x405
#define GL_FRONT_AND_BACK 0x0408
#define GL_CULL_FACE 0xB44
#define GL_DEPTH_TEST 0xB71
#define GL_STENCIL_TEST 0xB90
#define GL_BLEND 0xBE2
#define GL_TEXTURE_2D 0xDE1
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_SHORT 0x1403
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_DEPTH_COMPONENT 0x1902
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_POINT 0x1B00
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02
#define GL_KEEP 0x1E00
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_REPEAT 0x2901
#define GL_COLOR_BUFFER_BIT 0x4000
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_MULTISAMPLE 0x809D
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_TEXTURE_MAX_LEVEL 0x813D
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF
#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_INCR_WRAP 0x8507
#define GL_DECR_WRAP 0x8508
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STREAM_DRAW 0x88E0
#define GL_STATIC_DRAW 0x88E4
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_COMPILE_STATUS 0x8B81
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_CURRENT_PROGRAM 0x8B8D
#define GL_SRGB 0x8C40
#define GL_SRGB_ALPHA 0x8C42
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_FRAMEBUFFER 0x8D40
#define GL_GEOMETRY_SHADER 0x8DD9

//Function-pointer types
typedef void (*PFNGLACTIVETEXTUREPROC)(GLenum texture);
typedef void (*PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (*PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (*PFNGLBINDFRAMEBUFFERPROC)(GLenum target, GLuint framebuffer);
typedef void (*PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void (*PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void (*PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
typedef void (*PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void (*PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);
typedef void (*PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n, const GLuint *framebuffers);
typedef GLenum (*PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
typedef void (*PFNGLCLEARPROC)(GLbitfield mask);
typedef void (*PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (*PFNGLCOLORMASKPROC)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (*PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void (*PFNGLCOMPRESSEDTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
typedef GLuint(*PFNGLCREATEPROGRAMPROC)(void);
typedef GLuint(*PFNGLCREATESHADERPROC)(GLenum type);
typedef void (*PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void (*PFNGLDELETESHADERPROC)(GLuint shader);
typedef void (*PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint *textures);
typedef void (*PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint *arrays);
typedef void (*PFNGLDEPTHFUNCPROC)(GLenum func);
typedef void (*PFNGLDEPTHMASKPROC)(GLboolean flag);
typedef void (*PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (*PFNGLDISABLEPROC)(GLenum cap);
typedef void (*PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (*PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
typedef void (*PFNGLDRAWBUFFERPROC)(GLenum buf);
typedef void (*PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void (*PFNGLENABLEPROC)(GLenum cap);
typedef void (*PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (*PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (*PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
typedef void (*PFNGLGENFRAMEBUFFERSPROC)(GLsizei n, GLuint *framebuffers);
typedef void (*PFNGLGENTEXTURESPROC)(GLsizei n, GLuint *textures);
typedef void (*PFNGLGENERATEMIPMAPPROC)(GLenum target);
typedef void (*PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint* arrays);
typedef GLint (*PFNGLGETATTRIBLOCATIONPROC)(GLuint program, const GLchar *name);
typedef GLenum(*PFNGLGETERRORPROC)(void);
typedef void (*PFNGLGETINTEGERVPROC)(GLenum pname, GLint *data);
typedef void (*PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint *params);
typedef void (*PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (*PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
typedef void (*PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void(*PFNGLGETTEXPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
typedef GLint(*PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar *name);
typedef void (*PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (*PFNGLPOLYGONMODEPROC)(GLenum face, GLenum mode);
typedef void (*PFNGLREADBUFFERPROC) (GLenum src);
typedef void (*PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (*PFNGLSTENCILFUNCPROC)(GLenum func, GLint ref, GLuint mask);
typedef void (*PFNGLSTENCILMASKPROC)(GLuint mask);
typedef void (*PFNGLSTENCILOPPROC)(GLenum fail, GLenum zfail, GLenum zpass);
typedef void (*PFNGLSTENCILOPSEPARATEPROC)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (*PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (*PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (*PFNGLTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (*PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void (*PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void (*PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (*PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void (*PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void (*PFNGLUNIFORM1UIPROC)(GLint location, GLuint v0);
typedef void (*PFNGLUNIFORM2FVPROC)(GLint location, GLsizei count, const GLfloat *value);
typedef void (*PFNGLUNIFORM3FVPROC)(GLint location, GLsizei count, const GLfloat *value);
typedef void (*PFNGLUNIFORM4FVPROC)(GLint location, GLsizei count, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

//Function pointers
extern ACSTDLIB_API PFNGLACTIVETEXTUREPROC glActiveTexture;
extern ACSTDLIB_API PFNGLATTACHSHADERPROC glAttachShader;
extern ACSTDLIB_API PFNGLBINDBUFFERPROC glBindBuffer;
extern ACSTDLIB_API PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern ACSTDLIB_API PFNGLBINDTEXTUREPROC glBindTexture;
extern ACSTDLIB_API PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern ACSTDLIB_API PFNGLBLENDFUNCPROC glBlendFunc;
extern ACSTDLIB_API PFNGLBUFFERDATAPROC glBufferData;
extern ACSTDLIB_API PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
extern ACSTDLIB_API PFNGLCLEARPROC glClear;
extern ACSTDLIB_API PFNGLCLEARCOLORPROC glClearColor;
extern ACSTDLIB_API PFNGLCOLORMASKPROC glColorMask;
extern ACSTDLIB_API PFNGLCOMPILESHADERPROC glCompileShader;
extern ACSTDLIB_API PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
extern ACSTDLIB_API PFNGLCREATEPROGRAMPROC glCreateProgram;
extern ACSTDLIB_API PFNGLCREATESHADERPROC glCreateShader;
extern ACSTDLIB_API PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern ACSTDLIB_API PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern ACSTDLIB_API PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern ACSTDLIB_API PFNGLDELETESHADERPROC glDeleteShader;
extern ACSTDLIB_API PFNGLDELETETEXTURESPROC glDeleteTextures;
extern ACSTDLIB_API PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern ACSTDLIB_API PFNGLDEPTHFUNCPROC glDepthFunc;
extern ACSTDLIB_API PFNGLDEPTHMASKPROC glDepthMask;
extern ACSTDLIB_API PFNGLDETACHSHADERPROC glDetachShader;
extern ACSTDLIB_API PFNGLDISABLEPROC glDisable;
extern ACSTDLIB_API PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern ACSTDLIB_API PFNGLDRAWARRAYSPROC glDrawArrays;
extern ACSTDLIB_API PFNGLDRAWBUFFERPROC glDrawBuffer;
extern ACSTDLIB_API PFNGLDRAWELEMENTSPROC glDrawElements;
extern ACSTDLIB_API PFNGLENABLEPROC glEnable;
extern ACSTDLIB_API PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern ACSTDLIB_API PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
extern ACSTDLIB_API PFNGLGENBUFFERSPROC glGenBuffers;
extern ACSTDLIB_API PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern ACSTDLIB_API PFNGLGENTEXTURESPROC glGenTextures;
extern ACSTDLIB_API PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern ACSTDLIB_API PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern ACSTDLIB_API PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern ACSTDLIB_API PFNGLGETERRORPROC glGetError;
extern ACSTDLIB_API PFNGLGETINTEGERVPROC glGetIntegerv;
extern ACSTDLIB_API PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern ACSTDLIB_API PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern ACSTDLIB_API PFNGLGETSHADERIVPROC glGetShaderiv;
extern ACSTDLIB_API PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern ACSTDLIB_API PFNGLGETTEXPARAMETERIVPROC glGetTexParameteriv;
extern ACSTDLIB_API PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern ACSTDLIB_API PFNGLLINKPROGRAMPROC glLinkProgram;
extern ACSTDLIB_API PFNGLPOLYGONMODEPROC glPolygonMode;
extern ACSTDLIB_API PFNGLREADBUFFERPROC glReadBuffer;
extern ACSTDLIB_API PFNGLSHADERSOURCEPROC glShaderSource;
extern ACSTDLIB_API PFNGLSTENCILFUNCPROC glStencilFunc;
extern ACSTDLIB_API PFNGLSTENCILMASKPROC glStencilMask;
extern ACSTDLIB_API PFNGLSTENCILOPPROC glStencilOp;
extern ACSTDLIB_API PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate;
extern ACSTDLIB_API PFNGLTEXIMAGE2DPROC glTexImage2D;
extern ACSTDLIB_API PFNGLTEXPARAMETERIPROC glTexParameteri;
extern ACSTDLIB_API PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D;
extern ACSTDLIB_API PFNGLUSEPROGRAMPROC glUseProgram;
extern ACSTDLIB_API PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern ACSTDLIB_API PFNGLVIEWPORTPROC glViewport;
extern ACSTDLIB_API PFNGLUNIFORM1FPROC glUniform1f;
extern ACSTDLIB_API PFNGLUNIFORM1IPROC glUniform1i;
extern ACSTDLIB_API PFNGLUNIFORM1UIPROC glUniform1ui;
extern ACSTDLIB_API PFNGLUNIFORM2FVPROC glUniform2fv;
extern ACSTDLIB_API PFNGLUNIFORM3FVPROC glUniform3fv;
extern ACSTDLIB_API PFNGLUNIFORM4FVPROC glUniform4fv;
extern ACSTDLIB_API PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
extern ACSTDLIB_API PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
extern ACSTDLIB_API PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;