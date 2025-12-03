/* Copyright (c) 2025-present Henri Michelon
*
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */
export module lysa.resources.samplers;

import std;
import vireo;
import lysa.types;

export namespace lysa {

    /**
     * Manages a small, shared collection of GPU sampler objects.
     *  - Provide a fixed-size pool of samplers that materials/shaders can reuse.
     *  - Create and track descriptor layout and descriptor set for binding samplers.
     *  - Expose an update mechanism to (re)write descriptors when the set changes.
     */
    class Samplers {
    public:
        /** Descriptor set index used by pipelines to bind the samplers set. */
        static constexpr uint32 SET{1};

        static constexpr unique_id NEAREST_NEAREST_CLAMP_TO_BORDER = 0;
        static constexpr unique_id LINEAR_LINEAR_CLAMP_TO_EDGE = 1;
        static constexpr unique_id LINEAR_LINEAR_CLAMP_TO_EDGE_LOD_CLAMP_NONE = 2;
        static constexpr unique_id LINEAR_LINEAR_REPEAT = 3;
        static constexpr unique_id NEAREST_NEAREST_REPEAT = 4;

        /**
         * Human-readable description of sampler creation parameters.
         * Two SamplerInfo values are considered equal if all fields match;
         * this allows avoiding duplicate samplers.
         */
        struct SamplerInfo {
            /** Minification filter used by the sampler. */
            vireo::Filter minFilter;
            /** Magnification filter used by the sampler. */
            vireo::Filter maxFilter;
            /** Addressing mode for the U (S) texture coordinate. */
            vireo::AddressMode samplerAddressModeU;
            /** Addressing mode for the V (T) texture coordinate. */
            vireo::AddressMode samplerAddressModeV;
            /** Minimum mip level to sample from (LOD clamp). */
            float minLod;
            /** Maximum mip level to sample from (LOD clamp). */
            float maxLod;
            /** Enables anisotropic filtering if supported by the backend. */
            bool anisotropyEnable;
            /** Mip level sampling mode (nearest/linear). */
            vireo::MipMapMode mipMapMode;
            /** Optional comparison operation (useful for shadow samplers). */
            vireo::CompareOp samplerCompareOp;

            friend bool operator==(const SamplerInfo&l, const SamplerInfo&r) {
                return l.minFilter == r.minFilter &&
                    l.maxFilter == r.maxFilter &&
                    l.samplerAddressModeU == r.samplerAddressModeU &&
                    l.samplerAddressModeV == r.samplerAddressModeV &&
                    l.minLod == r.minLod &&
                    l.maxLod == r.maxLod &&
                    l.anisotropyEnable == r.anisotropyEnable &&
                    l.mipMapMode == r.mipMapMode &&
                    l.samplerCompareOp == r.samplerCompareOp;
            }
        };

        /**
         * Adds a sampler to the pool, creating it if an equivalent one does not
         * already exist, and returns its index for binding.
         *
         * @param minFilter          Minification filter.
         * @param maxFilter          Magnification filter.
         * @param samplerAddressModeU Address mode for U (S) coordinate.
         * @param samplerAddressModeV Address mode for V (T) coordinate.
         * @param minLod             Minimum LOD to sample (default 0.0f).
         * @param maxLod             Maximum LOD to sample; use Sampler::LOD_CLAMP_NONE to disable clamping.
         * @param anisotropyEnable   Enable anisotropic filtering when available.
         * @param mipMapMode         Mip sampling mode (default LINEAR).
         * @param compareOp          Optional compare op for depth/shadow lookups.
         * @return Index into the samplers set suitable for descriptor binding.
         */
        uint32 addSampler(
            vireo::Filter minFilter,
            vireo::Filter maxFilter,
            vireo::AddressMode samplerAddressModeU,
            vireo::AddressMode samplerAddressModeV,
            float minLod = 0.0f,
            float maxLod = vireo::Sampler::LOD_CLAMP_NONE,
            bool anisotropyEnable = true,
            vireo::MipMapMode mipMapMode = vireo::MipMapMode::LINEAR,
            vireo::CompareOp compareOp = vireo::CompareOp::NEVER);

        /** Returns true if the samplers descriptor set needs to be updated. */
        bool isUpdateNeeded() const { return samplersUpdated; }

        /** Writes pending sampler bindings to the descriptor set if needed. */
        void update();

        /** Returns the descriptor layout used for the samplers set. */
        const auto& getDescriptorLayout() const { return descriptorLayout; }

        /** Returns the descriptor set that binds the samplers to shaders. */
        const auto& getDescriptorSet() const { return descriptorSet; }

        /** Creates a Samplers pool bound to the given backend. */
        Samplers(const std::shared_ptr<vireo::Vireo>& vireo, size_t capacity);

        ~Samplers();

    private:
        /** Reference to the graphics backend entry point. */
        const std::shared_ptr<vireo::Vireo> vireo;
        /** Number of active samplers in the pool. */
        uint32 samplerCount{0};
        /** Sampler objects owned by this pool. */
        std::vector<std::shared_ptr<vireo::Sampler>> samplers;
        /** Cached creation parameters to detect duplicates and changes. */
        std::vector<SamplerInfo>                     samplersInfo;
        /** Descriptor layout describing the samplers binding. */
        std::shared_ptr<vireo::DescriptorLayout>     descriptorLayout;
        /** Descriptor set that holds the array of samplers. */
        std::shared_ptr<vireo::DescriptorSet>        descriptorSet;
        /** Flag set when samplers were added/changed and descriptors must be updated. */
        bool samplersUpdated{false};
        /** Mutex that guards modifications to the pool and descriptor set. */
        std::mutex mutex;
    };

}