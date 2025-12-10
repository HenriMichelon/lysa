/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.ecs.components;

import vireo;
import lysa.aabb;
import lysa.context;
import lysa.math;

export import lysa.ecs.components.transform;
export import lysa.ecs.flecs;
import lysa.renderers.graphic_pipeline_data;

export namespace lysa::ecs {

    struct Updated { };

    struct Context {
        lysa::Context* ctx;
    };

    struct RenderTarget {
        unique_id renderTarget{INVALID_ID};
    };

    struct Viewport {
        //! Low‑level viewport (x, y, width, height, minDepth, maxDepth).
        vireo::Viewport viewport{};
        //! Scissors rectangle limiting rendering to a sub‑area.
        vireo::Rect scissors{};
    };

    struct Camera {
        // Is the projection perspective?
        bool isPerspective{true};
        // Field of view in degrees
        float fov{75.0};
        // Camera view aspect ratio
        float aspectRatio{16.0f / 9.0f};
        // Nearest clipping distance
        float near{0.05f};
        // Furthest clipping distance
        float far{100.0f};
        // left of orthographic projection:
        float left{};
        // left of orthographic projection:
        float right{};
        // top of orthographic projection:
        float top{};
        // bottom of orthographic projection:
        float bottom{};
        // World projection matrix
        float4x4 projection{float4x4::identity()};
    };

    struct CameraRef {
        flecs::entity camera;
    };

    struct MeshInstance {
        unique_id mesh{INVALID_ID};
        bool visible{true};
        bool castShadows{false};
        AABB worldAABB;
    };

    struct Scene {
        unique_id scene{INVALID_ID};
    };

    struct SceneRef {
        flecs::entity scene;
    };

}
