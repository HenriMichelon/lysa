# Lysa

Lysa is a hobby 3D engine created for learning and experimenting low level
graphics programming and game engines foundations.

# Features
- GPU-driven forward and deferred renderers.
- Multi graphic API support (Vulkan and DirectX 12 using [Vireo RHI](https://github.com/HenriMichelon/vireo_rhi))
- Multi physics API support ([Jolt physics](https://github.com/jrouwe/JoltPhysics) and Nvidia Physx)
- [Lua](https://lua.org/) bindings
- Bundled with [Slang](https://shader-slang.org/) shaders featuring PBR (simplified), Bloom, SSAO, HDR tone-mapping (Reinhard and ACES), SMAA, FXAA.
- Written in C++ 23.

# Optional features
- [ECS](https://github.com/HenriMichelon/lysa_ecs) addon using [Flecs](https://www.flecs.dev/flecs/)
