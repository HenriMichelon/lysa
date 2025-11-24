/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.render_target;

import vireo;
import lua_bridge;

import lysa.context;
import lysa.lua;
import lysa.types;
import lysa.resources.manager;

export namespace lysa {

    struct RenderTargetConfiguration {
        //! Set this field if you want to render in a window
        void* renderingWindowHandle{nullptr};
        //! Postprocessing & swap chain image format
        vireo::ImageFormat swapChainFormat{vireo::ImageFormat::R8G8B8A8_UNORM};
        //! Presentation mode
        vireo::PresentMode presentMode{vireo::PresentMode::IMMEDIATE};
        //! Number of simultaneous frames during rendering
        uint32 framesInFlight{2};
    };

    struct RenderTarget {
        struct FrameData {
            /** Fence signaled when the frame's work has completed on GPU. */
            std::shared_ptr<vireo::Fence> inFlightFence;
            /** Command allocator for this frame (resets between frames). */
            std::shared_ptr<vireo::CommandAllocator> commandAllocator;
            /** Command list used for rendering into the swap chain. */
            std::shared_ptr<vireo::CommandList> renderCommandList;
        };

        //! Unique ID
        unique_id id{INVALID_ID};
        //! Set to true to pause the rendering in this target
        bool paused{false};
        /** Array of per‑frame resource bundles (size = frames in flight). */
        std::vector<FrameData> framesData;
        //! Swap chain presenting the render target in memory.
        std::shared_ptr<vireo::SwapChain> swapChain{nullptr};
    };

    class RenderTargetManager : public ResourcesManager<RenderTarget> {
    public:
        static constexpr auto ID = "RenderTarget";

        /**
         * @brief Construct a manager bound to the given runtime context.
         * @param ctx Instance wide context
         * @param capacity Initial capacity
         */
        RenderTargetManager(Context& ctx, unique_id capacity);

        /**
         * @brief Create a new rendering render target
         * @param configuration Render target creation parameters
         * @return The unique @ref unique_id of the newly render target.
         */
        unique_id create(const RenderTargetConfiguration& configuration);

        void destroy(RenderTarget& renderTarget) override;

    private:
        friend class Lysa;
        friend class ResourcesLocator;

        void update() const;

        void drawFrame() const;

        static void _register(const Lua& lua);
    };

}

