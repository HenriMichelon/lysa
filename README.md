# Lysa Engine

Lysa Engine is a hobby 3D engine created for learning and experimenting with low-level graphics programming and game engine foundations.

## Features

- GPU-driven forward and deferred renderers.
- Vulkan and DirectX 12 support through [Vireo RHI](https://github.com/HenriMichelon/vireo_rhi).
- Built with C++23, utilizing C++ modules for clean architecture.
- Integrated with [Slang](https://shader-slang.org/) shaders.
    - PBR (Simplified)
    - Post-processing: Bloom, SSAO, FXAA, SMAA
    - HDR Tone-mapping (Reinhard and ACES)
- [Lua](https://lua.org/) bindings for scripting.

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
set(PHYSIC_ENGINE_JOLT ON)
set(FORWARD_RENDERER ON)
set(DEFERRED_RENDERER ON)
add_subdirectory(path/to/lysa_engine)
target_link_libraries(your_target PUBLIC lysa_engine)
```

## Additional features

### Lysa UI

Lysa Engine is designed to work seamlessly with **[Lysa UI](https://github.com/HenriMichelon/lysa_ui)**, a C++23 user interface library.


