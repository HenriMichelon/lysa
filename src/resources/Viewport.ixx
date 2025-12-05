/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.viewport;

import vireo;

import lysa.context;
import lysa.types;
import lysa.resources.resource_manager;

export namespace lysa {

    struct ViewportConfiguration : ResourceConfiguration {
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

        auto getRenderTarget() const { return renderTarget; }

    private:
        Context& ctx;
        //! Parent render target
        unique_id renderTarget{INVALID_ID};
        //! Low‑level viewport (x, y, width, height, minDepth, maxDepth).
        vireo::Viewport viewport{};
        //! Scissors rectangle limiting rendering to a sub‑area.
        vireo::Rect scissors{};
        //! Configuration used when creating the viewport
        ViewportConfiguration configuration;

        friend class ViewportManager;
        void resize(const vireo::Extent &extent);
    };

    class ViewportManager : public ResourcesManager<Viewport> {
    public:
        /**
         * @brief Construct a manager bound to the given runtime context.
         * @param ctx Instance wide context
         * @param capacity Initial capacity
         */
        ViewportManager(Context& ctx, size_t capacity);

        ~ViewportManager() override {
            cleanup();
        }

        Viewport& create(const ViewportConfiguration& configuration);


    private:
        auto getResources(unique_id renderTarget) {
            return resources | std::views::filter([renderTarget](auto& res) {
                return res != nullptr && res->renderTarget == renderTarget;
            });
        }

        friend class RenderTarget;
        void resize(unique_id renderTarget, const vireo::Extent &extent);
        void destroyByRenderTarget(unique_id renderTarget);

    };

}

