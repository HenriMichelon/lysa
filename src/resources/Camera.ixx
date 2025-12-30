/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.camera;

import vireo;

import lysa.context;
import lysa.math;
import lysa.resources.image;
import lysa.resources.manager;

export namespace lysa {

    /**
     * Base class for camera resources.
     */
    struct Camera : Resource {
        /** World space position */
        float3 position;
        /** World space transform */
        float4x4 transform;
        /** View projection */
        float4x4 projection;

        Camera(Context&, const float3& position, const float4x4& transform, const float4x4& projection) :
            position(position),
            transform(transform),
            projection(projection) {}
    };

    class CameraManager : public ResourcesManager<Context, Camera> {
    public:
        /**
         * @brief Construct a manager bound to the given runtime context.
         * @param ctx Instance wide context
         * @param capacity Initial capacity
         */
        CameraManager(Context& ctx, const unique_id capacity) :
            ResourcesManager(ctx, capacity, "CameraManager") {
            ctx.res.enroll(*this);
        }

    };

}

