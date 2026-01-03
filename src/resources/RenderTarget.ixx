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
import lysa.resources;
import lysa.resources.render_view;
import lysa.resources.camera;
import lysa.resources.scene;

export namespace lysa {

    struct RenderTargetConfiguration {
        //! Postprocessing & swap chain image format
        vireo::ImageFormat swapChainFormat{vireo::ImageFormat::R8G8B8A8_UNORM};
        //! Presentation mode
        vireo::PresentMode presentMode{vireo::PresentMode::IMMEDIATE};
        //! Number of simultaneous frames during rendering
        uint32 framesInFlight{2};
        //! Configuration for the rendering path of the target
        RendererConfiguration rendererConfiguration;
    };

    /**
    * Render target events data
    */
    struct RenderTargetEvent {
        //! The render target has been paused
        static constexpr auto PAUSED{"RENDERING_TARGET_PAUSED"};
        //! The render target has been resumed
        static constexpr auto RESUMED{"RENDERING_TARGET_RESUMED"};
        //! The render target has been resized
        static constexpr auto RESIZED{"RENDERING_TARGET_RESIZED"};
    };

    //! Platform specific handle/ID
    using RenderingWindowHandle = void*;

    class RenderTarget : public UniqueResource {
    public:
        RenderTarget(Context& ctx, const RenderTargetConfiguration& configuration, RenderingWindowHandle renderingWindowHandle);

        ~RenderTarget() override;

        void render();

        auto isPaused() const { return paused; }

        void setPause(bool pause);

        float getAspectRatio() const { return swapChain->getAspectRatio(); }

        uint32 getFramesInFlight() const { return swapChain->getFramesInFlight(); }

        auto getSwapChain() const { return swapChain; }

        void waitIdle() const { swapChain->waitIdle(); }

        void addView(const RenderView& view);

        void removeView(const RenderView& view);

        void updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) const;

        void resize();

        Context& getContext() const { return ctx; }

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
        SceneManager& sceneManager;
        // Set to true to pause the rendering in this target
        bool paused{false};
        // Array of per‑frame resource bundles (size = frames in flight).
        std::vector<FrameData> framesData;
        // Swap chain presenting the render target in memory.
        std::shared_ptr<vireo::SwapChain> swapChain{nullptr};
        // Scene renderer used to draw attached viewports.
        std::unique_ptr<Renderer> renderer;
        // Views to render in this target
        std::list<RenderView> views;
    };

}

