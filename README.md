# Std++
Std++ is an attempt to create a strong cross-platform standard library for C++.

## Support

C++17 is a strict requirement.

Supported operating systems:
* Linux
* Windows 7
* Windows 10

Supported compilers:
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

The following Compute-backends are currently implemented:
* OpenCL

The following Render-backends are currently implemented:
* OpenGL

The following UI-backends are currently implemented:
* GTK 3
* Common Controls Library (Windows)

## Dependencies

### Mandatory dependencies

None.

### Optional dependencies

* OpenCL ICD loader for the OpenCL backend.
* GTK 3 and PkgConfig for the GTK 3 backend.