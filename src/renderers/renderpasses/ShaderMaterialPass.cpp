/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module lysa.renderers.renderpasses.shader_material_pass;

import lysa.renderers.graphic_pipeline_data;

namespace lysa {
    ShaderMaterialPass::ShaderMaterialPass(
        const Context& ctx,
        const RendererConfiguration& config):
        Renderpass{ctx, config, "ShaderMaterialPass"},
        materialManager(ctx.res.get<MaterialManager>()) {

        pipelineConfig.colorRenderFormats.push_back(config.colorRenderingFormat);
        pipelineConfig.depthStencilImageFormat = config.depthStencilFormat;
        pipelineConfig.resources = ctx.vireo->createPipelineResources({
            ctx.globalDescriptorLayout,
            ctx.samplers.getDescriptorLayout(),
            SceneFrameData::sceneDescriptorLayout,
            GraphicPipelineData::pipelineDescriptorLayout,
#ifdef SHADOW_TRANSPARENCY_COLOR_ENABLED
            SceneFrameData::sceneDescriptorLayoutOptional1
#endif
            },
            SceneFrameData::instanceIndexConstantDesc, name);
        pipelineConfig.vertexInputLayout = ctx.vireo->createVertexLayout(sizeof(VertexData), VertexData::vertexAttributes);

        renderingConfig.colorRenderTargets[0].clearValue = {
            config.clearColor.r,
            config.clearColor.g,
            config.clearColor.b,
            1.0f};
        renderingConfig.clearDepthStencil = false; //!config.forwardDepthPrepass;
    }

    void ShaderMaterialPass::updatePipelines(const std::unordered_map<pipeline_id, std::vector<unique_id>>& pipelineIds) {
        for (const auto& [pipelineId, materials] : pipelineIds) {
            if (!pipelines.contains(pipelineId)) {
                const auto& material = materialManager[materials.at(0)];
                std::string vertShaderName = DEFAULT_VERTEX_SHADER;
                std::string fragShaderName = DEFAULT_FRAGMENT_SHADER;
                if (material.getType() == Material::SHADER) {
                    const auto& shaderMaterial = dynamic_cast<const ShaderMaterial&>(material);
                    if (!shaderMaterial.getVertFileName().empty()) {
                        vertShaderName = shaderMaterial.getVertFileName();
                    }
                    if (!shaderMaterial.getFragFileName().empty()) {
                        fragShaderName = shaderMaterial.getFragFileName();
                    }
                }
                pipelineConfig.cullMode = material.getCullMode();
                pipelineConfig.vertexShader = loadShader(vertShaderName);
                pipelineConfig.fragmentShader = loadShader(fragShaderName);
                pipelines[pipelineId] = ctx.vireo->createGraphicPipeline(pipelineConfig, name);
            }
        }
    }

    void ShaderMaterialPass::render(
        vireo::CommandList& commandList,
        const SceneFrameData& scene,
        const std::shared_ptr<vireo::RenderTarget>& colorAttachment,
        const std::shared_ptr<vireo::RenderTarget>& depthAttachment,
        const bool clearAttachment,
        const uint32) {
        renderingConfig.colorRenderTargets[0].clear = clearAttachment;
        renderingConfig.colorRenderTargets[0].renderTarget = colorAttachment;
        renderingConfig.depthStencilRenderTarget = depthAttachment;

        commandList.barrier(
            colorAttachment,
            vireo::ResourceState::UNDEFINED,
            vireo::ResourceState::RENDER_TARGET_COLOR);
        commandList.beginRendering(renderingConfig);
        scene.drawShaderMaterialModels(
            commandList,
            pipelines);
        commandList.endRendering();
        commandList.barrier(
            colorAttachment,
            vireo::ResourceState::RENDER_TARGET_COLOR,
            vireo::ResourceState::UNDEFINED);
    }
}