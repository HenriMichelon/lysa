/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.viewport;

import vireo;
import lua_bridge;

import lysa.context;
import lysa.lua;
import lysa.types;
import lysa.resources.manager;

export namespace lysa {

    struct ViewportConfiguration {
        //! Parent render target
        unique_id renderTarget{INVALID_ID};
        //! Low‑level viewport (x, y, width, height, minDepth, maxDepth).
        vireo::Viewport viewport{};
        //! Scissors rectangle limiting rendering to a sub‑area.
        vireo::Rect scissors{};
    };

    class Viewport : public Resource {
    public:
        Viewport(Context& ctx, const ViewportConfiguration& configuration);

        ~Viewport();

        void _resize(const vireo::Extent &extent);

        auto getRenderTarget() const { return renderTarget; }

    private:
        /** Per‑frame state and deferred operations processed at frame boundaries. */
        struct FrameData {

        };
        //! Parent render target
        unique_id renderTarget{INVALID_ID};
        //! Low‑level viewport (x, y, width, height, minDepth, maxDepth).
        vireo::Viewport viewport{};
        //! Scissors rectangle limiting rendering to a sub‑area.
        vireo::Rect scissors{};
        /** Array of per‑frame resource bundles (size = frames in flight). */
        std::vector<FrameData> framesData;
        //! Configuration used when creating the viewport
        ViewportConfiguration configuration;
    };

    class ViewportManager : public ResourcesManager<Viewport> {
    public:
        static constexpr auto ID = "Viewport";

        /**
         * @brief Construct a manager bound to the given runtime context.
         * @param ctx Instance wide context
         * @param capacity Initial capacity
         */
        ViewportManager(Context& ctx, unique_id capacity);

        /**
         * @brief Create a new rendering render target
         * @param configuration Render target creation parameters
         * @return The unique @ref unique_id of the newly render target.
         */
        Viewport create(const ViewportConfiguration& configuration);

//        void destroy(unique_id renderTarget);

    private:
        friend class RenderTargetManager;
        friend class ResourcesLocator;

        void resize(unique_id renderTarget, const vireo::Extent &extent);

        static void _register(const Lua& lua);
    };

}

