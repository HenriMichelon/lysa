/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.render_view;

import vireo;

import lysa.context;
import lysa.types;
import lysa.resources.manager;

export namespace lysa {

    struct RenderView : Resource {
        vireo::Viewport viewport{};
        vireo::Rect scissors{};
        unique_id camera{INVALID_ID};
        unique_id scene{INVALID_ID};
    };

    class RenderViewManager : public ResourcesManager<Context, RenderView> {
    public:
        /**
         * @brief Construct a manager bound to the given runtime context.
         * @param ctx Instance wide context
         * @param capacity Initial capacity
         */
        RenderViewManager(Context& ctx, const unique_id capacity) :
            ResourcesManager(ctx, capacity, "RenderViewManager") {
            ctx.res.enroll(*this);
        }
    };

}

