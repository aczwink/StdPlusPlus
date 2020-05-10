# Std++
Std++ is an attempt to create a strong cross-platform standard library for C++.

## Support

C++17 is a strict requirement.

### Supported operating systems
* Basically any POSIX-compliant OS (i.e. Linux, Darwin (macOS), any kind of Unix) is supported.
* Linux (i.e. additional features that aren't covered by POSIX)
* Windows 7
* Windows 10

### Supported compilers
* Clang
* GCC
* Visual Studio

Other systems and compilers might be supported as well but this was not tested.

## Backends
Several features of Std++ are split into frontends and backends.
The programmer writes his code using the frontend (without having to know [to some extent] the backend), while Std++ maps the calls made to the backend.
A good example is the UI-module of Std++ that can interact with several GUI-libraries/Toolkits/APIs etc.
Not only does this approach make Std++ more flexible but this also helps in achieving the cross-platform goal.
With this design decision, it is for example not mandatory to compile Std++ with a GUI-backend.
Like this, code is more reusable.

The following Audio-backends are currently implemented:
* OpenAL Soft
* XAudio2

The following Compute-backends are currently implemented:
* OpenCL 1

The following Render-backends are currently implemented:
* OpenGL 3 Core Profile

The following UI-backends are currently implemented:
* Cocoa (macOS)
* Common Controls Library (Windows)
* GTK 3
* XCB-Xlib (Xlib is needed for GLX for example)

## Extensions
Similar to backends, there are extensions. The difference is saddle from the users perspective.
Extensions extend the possibilities of the library (add cryptographic functions, codecs etc.).
The library functions without them but offers more possibilities when having them.

The following extensions are currently implemented:
* fuse - for mounting filesystems into the operating systems file system
* libavcodec (ffmpeg) - provides a huge amount of codecs
* libavformat (ffmpeg) - provides a huge amount of multimedia container formats
* liblzma (XZ Utils) - for compressing and decompressing using the LZMA algorithm
* OpenSSL - for cryptography ciphers and cryptographic hash functions
* zlib - for compression and decompression using the Deflate algorithm

## Dependencies

### Mandatory dependencies

None.
Std++ was designed to be flexible and does not depend on anything your OS does not provide.

### Optional dependencies

If you want to use any of the following backends, you will need to install them:
* OpenAL Soft
* OpenCL 1.x ICD loader
* GTK 3 and PkgConfig

Extensions are usually not available by default and you have to install the corresponding software package in order to use it.