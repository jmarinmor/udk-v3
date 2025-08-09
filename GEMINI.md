# Uncannymoon Game Engine

## 1. Project Overview

Uncannymoon is a real-time graphics engine for video games and real time applications. It is designed to be a flexible and extensible platform that can be used as a dynamic or static library in other projects.

## 2. Core Technologies

- **Core Language:** C/C++
- **Build System:** CMake
- **Graphics APIs:** Vulkan (primary), OpenGL, DirectX, Metal
- **Audio APIs:** OpenAL, OpenSL
- **Compute APIs:** OpenCL
- **Key Libraries:**
    - **Assimp:** For importing 3D models.
    - **GLAD:** For OpenGL function loading.
    - **GLFW:** For windowing and input.
    - **Freetype:** For font rendering.

## 3. Platform Support

The engine is designed to be cross-platform and will support the following operating systems:

- Windows
- macOS
- Linux
- Android
- iOS

## 4. Architecture

The architecture is based on a modular design with a strong emphasis on interfaces and adapters.

- **Core:** The main engine code, written in C++. It defines the core interfaces for graphics, audio, and other systems.
- **Adapters:** C++ implementations of the core interfaces for different graphics APIs and platforms. This allows for a high degree of flexibility and platform-specific optimizations.
- **Bindings:** A C-style API will be created to allow the engine to be used from other languages. This will be the foundation for creating bindings for languages like Java, C#, Python, and Swift.

## 5. Directory Structure

```
.
├── adapters/         # C++ implementations of the core interfaces
├── bindings/         # Language bindings
│   ├── c/            # C API for creating bindings
│   ├── csharp/       # C# bindings
│   ├── java/         # Java bindings
│   ├── python/       # Python bindings
│   └── swift/        # Swift bindings
├── core/             # Core C++ engine code
│   ├── pre/          # Preprocessor directives
│   ├── core/         # Basic functionality (threads, time, formats, handles, simd, transformations, shapes, object definition, ...)
│   └── apu/          # Accelerated process units
├── dependencies/     # Third-party libraries
├── examples/         # C++ examples using the engine
├── examples_bindings/# Examples using the language bindings
├── project/          # Main CMake project for compiling the engine
└── tests/            # Unit tests
```

## 6. Development Workflow

- **CMake:** The primary build system. It will be used to generate project files for different IDEs and platforms.
- **Debugging:** CMake will be configured to generate projects for the engine, examples, and tests to facilitate debugging.

## 7. Language Bindings

- A C API will be created in the `bindings/c` directory.
- This C API will be used to create high-level bindings for other languages.
- The goal is to provide a seamless and idiomatic experience for developers using the engine from different languages.

## 8. Dependency Management

- Third-party libraries that cannot be found by CMake will be included in the `dependencies` directory.
- CMake will be configured to build these libraries as part of the main project.