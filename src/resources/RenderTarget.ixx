/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
export module lysa.resources.render_target;

import vireo;

import lysa.context;
import lysa.event;
import lysa.types;
import lysa.renderers.configuration;
import lysa.renderers.renderer;
import lysa.resources.resource_manager;
import lysa.resources.viewport;

export namespace lysa {

    struct RenderTargetConfiguration {
        //! Set this field if you want to render in a window
        void* renderingWindowHandle{nullptr};

        RendererConfiguration rendererConfiguration;
    };

    /**
   * Viewport events data
   */
    struct RenderTargetEvent : Event {
        //! The render target has been paused
        static constexpr auto PAUSED{"RENDERING_TARGET_PAUSED"};
        //! The render target has been resumed
        static constexpr auto RESUMED{"RENDERING_TARGET_RESUMED"};
        //! The render target has been resized
        static constexpr auto RESIZED{"RENDERING_TARGET_RESIZED"};
    };

    class RenderTarget : public Resource {
    public:
        RenderTarget(Context& ctx, const RenderTargetConfiguration& configuration);

        ~RenderTarget();

        void pause(bool pause);

        auto getSwapChain() const { return swapChain; }

        auto getRenderingWindowHandle() const { return renderingWindowHandle; }

        auto isPaused() const { return paused; }

    private:
        struct FrameData {
            /** Fence signaled when the frame's work has completed on GPU. */
            std::shared_ptr<vireo::Fence> inFlightFence;
            /** Command allocator for this frame (resets between frames). */
            std::shared_ptr<vireo::CommandAllocator> commandAllocator;
            /** Semaphore signaled when pre‑render stage is finished. */
            std::shared_ptr<vireo::Semaphore> prepareSemaphore;
            /** Command list used for rendering into the swap chain. */
            std::shared_ptr<vireo::CommandList> prepareCommandList;
            /** Command list used for rendering into the swap chain. */
            std::shared_ptr<vireo::CommandList> renderCommandList;
        };

        // Set to true to pause the rendering in this target
        bool paused{false};
        // Array of per‑frame resource bundles (size = frames in flight).
        std::vector<FrameData> framesData;
        // Swap chain presenting the render target in memory.
        std::shared_ptr<vireo::SwapChain> swapChain{nullptr};
        // Scene renderer used to draw attached viewports.
        std::unique_ptr<Renderer> renderer;
        // associated OS window handler
        void* renderingWindowHandle{nullptr};

        ViewportManager& viewportManager;

        friend class RenderTargetManager;
        void resize() const;
        void update() const;
        void render() const;
    };

    class RenderTargetManager : public ResourcesManager<RenderTarget> {
    public:
        /**
         * @brief Construct a manager bound to the given runtime context.
         * @param ctx Instance wide context
         * @param capacity Initial capacity
         */
        RenderTargetManager(Context& ctx, unique_id capacity);

        ~RenderTargetManager() override {
            cleanup();
        }

        void destroy(const void* renderingWindowHandle);

        void resize(const void* renderingWindowHandle) const;

        void pause(const void* renderingWindowHandle, bool pause);

    private:
        friend class Lysa;
        void update() const;
        void render() const;

        friend class ResourcesLocator;
    };

}

