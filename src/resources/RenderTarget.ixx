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
import lysa.input_event;
import lysa.math;
import lysa.renderers.configuration;
import lysa.renderers.graphic_pipeline_data;
import lysa.renderers.renderer;
import lysa.resources.camera;
import lysa.resources.manager;
import lysa.resources.render_view;
import lysa.resources.scene;

export namespace lysa {

    struct RenderTargetConfiguration {
        //! Set this field if you want to render in a window
        void* renderingWindowHandle{nullptr};
        //! Postprocessing & swap chain image format
        vireo::ImageFormat swapChainFormat{vireo::ImageFormat::R8G8B8A8_UNORM};
        //! Presentation mode
        vireo::PresentMode presentMode{vireo::PresentMode::IMMEDIATE};
        RendererConfiguration rendererConfiguration;
    };

    /**
    * Render target events data
    */
    struct RenderTargetEvent : Event {
        //! The render target has been paused
        static constexpr auto PAUSED{"RENDERING_TARGET_PAUSED"};
        //! The render target has been resumed
        static constexpr auto RESUMED{"RENDERING_TARGET_RESUMED"};
        //! The render target has been resized
        static constexpr auto RESIZED{"RENDERING_TARGET_RESIZED"};
        //! Input inside the render target parent window
        //static constexpr auto INPUT{"RENDERING_TARGET_INPUT"};
    };

    class RenderTarget : public Resource {
    public:
        RenderTarget(Context& ctx, const RenderTargetConfiguration& configuration, uint32 framesInFlight);

        ~RenderTarget() override;

        void render() const;

        void pause(bool pause);

        float getAspectRatio() const { return swapChain->getAspectRatio(); }

        auto getSwapChain() const { return swapChain; }

        auto getRenderingWindowHandle() const { return renderingWindowHandle; }

        auto isPaused() const { return paused; }

        void waitIdle() const { swapChain->waitIdle(); }

        void addView(unique_id viewId);

        void removeView(unique_id viewId);

        void updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) const;

    private:
        struct FrameData {
            /** Fence signaled when the frame's work has completed on GPU. */
            std::shared_ptr<vireo::Fence> inFlightFence;
            /** Command allocator for this frame (resets between frames). */
            std::shared_ptr<vireo::CommandAllocator> commandAllocator;
            /** Semaphore signaled when compute stage is finished. */
            std::shared_ptr<vireo::Semaphore> computeSemaphore;
            /** Semaphore signaled when pre‑render stage is finished. */
            std::shared_ptr<vireo::Semaphore> prepareSemaphore;
            /** Command list used for compute workloads. */
            std::shared_ptr<vireo::CommandList> computeCommandList;
            /** Command list used for rendering into the swap chain. */
            std::shared_ptr<vireo::CommandList> prepareCommandList;
            /** Command list used for rendering into the swap chain. */
            std::shared_ptr<vireo::CommandList> renderCommandList;
        };

        Context& ctx;
        RenderViewManager& renderViewManager;
        SceneManager& sceneManager;
        CameraManager& cameraManager;
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
        // Views to render in this target
        std::list<unique_id> views;

        friend class RenderTargetManager;
        void resize() const;
    };

    class RenderTargetManager : public ResourcesManager<Context, RenderTarget> {
    public:
        /**
         * @brief Construct a manager bound to the given runtime context.
         * @param ctx Instance wide context
         * @param capacity Initial capacity
         * @param framesInFlight
         */
        RenderTargetManager(Context& ctx, size_t capacity, uint32 framesInFlight);

        RenderTarget& create(const RenderTargetConfiguration& configuration);

        void destroy(const void* renderingWindowHandle);

        void resize(const void* renderingWindowHandle) const;

        void updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) const;

        void pause(const void* renderingWindowHandle, bool pause);

    private:
        uint32 framesInFlight;

        friend class Lysa;

        friend class ResourcesRegistry;
    };

}

