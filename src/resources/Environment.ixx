/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.environment;

import vireo;

import lysa.context;
import lysa.math;
import lysa.resources.manager;

export namespace lysa {

    struct Environment : Resource {
        float3 color{1.0f, 1.0f, 1.0f};
        float intensity{1.0f};

        Environment(Context&) {}
        Environment(
            Context&,
            const float3& color,
            const float intensity) :
            color(color),
            intensity(intensity) {}
    };

    class EnvironmentManager : public ResourcesManager<Context, Environment> {
    public:
        /**
         * @brief Construct a manager bound to the given runtime context.
         * @param ctx Instance wide context
         * @param capacity Initial capacity
         */
        EnvironmentManager(Context& ctx, const unique_id capacity) :
            ResourcesManager(ctx, capacity, "EnvironmentManager") {
            ctx.res.enroll(*this);
        }
    };

}

