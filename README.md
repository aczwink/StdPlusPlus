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

The following Compute-backends are currently implemented:
* OpenCL 1

The following Render-backends are currently implemented:
* OpenGL 3 Core Profile

The following UI-backends are currently implemented:
* Cocoa (macOS)
* GTK 3
* Common Controls Library (Windows)

## Dependencies

### Mandatory dependencies

None.

### Optional dependencies

* OpenAL Soft for the OpenAL Soft backend
* OpenCL 1.x ICD loader for the OpenCL 1 backend.
* GTK 3 and PkgConfig for the GTK 3 backend.