# <span style='color: red;'>**This is an old repository with a preliminary version. For an up to date projet see [Lysa](https://github.com/LysaEngine)**</span> #

# Lysa Engine

Lysa Engine is a hobby 3D engine created for learning and experimenting with low-level graphics programming and game engine foundations.

## Features

- **Hybrid Rendering**: GPU-driven forward and deferred renderers.
- **Advanced Shaders & Post-processing**: Integrated with [Slang](https://shader-slang.org/) shaders.
    - **PBR**: Simplified Physically Based Rendering.
    - **Transparency**: Weighted Blended Order-Independent Transparency (OIT).
    - **Shadows**: Support for Directional and Point light shadow maps.
    - **Culling**: GPU-driven Frustum Culling.
    - **Post-processing**: Bloom, SSAO, FXAA, SMAA, and HDR Tone-mapping (Reinhard/ACES).
- **Core Systems**:
    - **Asynchronous Task Pool**: Multi-threaded task execution and deferred command buffering.
    - **Event System**: Centralized observer-based event dispatcher.
    - **Virtual File System**: Portable path resolution using `app://` URI schemes.
    - **Logging**: Flexible logging to console, file, or virtual debug window.
- **Resource Management**: Dedicated managers for Meshes, Textures, and Materials with automatic GPU uploading.
- **Modern C++**: Built with C++23, utilizing C++ modules for clean architecture.
- **Multi-API Support**: Vulkan and DirectX 12 support through [Vireo RHI](https://github.com/HenriMichelon/vireo_rhi).
- **Scripting**: [Lua](https://lua.org/) bindings for high-level logic and rapid prototyping.

## Getting Started

### Prerequisites

- [Vulkan](https://vulkan.lunarg.com/) SDK
- [Vireo RHI](https://github.com/HenriMichelon/vireo_rhi)
- C++23 compatible compiler (MSVC, LLVM, etc.)
- CMake 3.29+ with a build tool like Ninja

### Integration

To use Lysa Engine in your project, you need to set the `VIREO_RHI_PROJECT_DIR` variable, usually in a `.env.cmake` file at your project root.

```cmake
# .env.cmake
set(VIREO_RHI_PROJECT_DIR "path/to/vireo_rhi")
```

Then, add it via CMake:

```cmake
set(LYSA_ENGINE_AS_DEPENDENCY ON)
set(LUA_BINDING ON)
set(DIRECTX_BACKEND ON)
set(FORWARD_RENDERER ON)
set(DEFERRED_RENDERER ON)
add_subdirectory(path/to/lysa_engine)
target_link_libraries(your_target PUBLIC lysa_engine)
```

## Additional features

### Lysa UI

Lysa Engine is designed to work seamlessly with **[Lysa UI](https://github.com/HenriMichelon/lysa_ui)**, a C++23 user interface library.


