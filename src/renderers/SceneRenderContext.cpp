/*
 * Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */
module lysa.renderers.scene_render_context;

import lysa.exception;
import lysa.log;
import lysa.resources.image;

namespace lysa {

    void SceneRenderContext::createDescriptorLayouts(
        const std::shared_ptr<vireo::Vireo>& vireo,
        const SceneRenderContextConfiguration& config) {
        sceneDescriptorLayout = vireo->createDescriptorLayout("Scene");
        sceneDescriptorLayout->add(BINDING_SCENE, vireo::DescriptorType::UNIFORM);
        sceneDescriptorLayout->add(BINDING_MODELS, vireo::DescriptorType::DEVICE_STORAGE);
        sceneDescriptorLayout->add(BINDING_LIGHTS, vireo::DescriptorType::UNIFORM);
        sceneDescriptorLayout->add(BINDING_SHADOW_MAPS, vireo::DescriptorType::SAMPLED_IMAGE,
            config.maxShadowMaps * 6);
        sceneDescriptorLayout->build();

        sceneDescriptorLayoutOptional1 = vireo->createDescriptorLayout("Scene opt1");
        sceneDescriptorLayoutOptional1->add(BINDING_SHADOW_MAP_TRANSPARENCY_COLOR,
            vireo::DescriptorType::SAMPLED_IMAGE, config.maxShadowMaps * 6);
        sceneDescriptorLayoutOptional1->build();

        GraphicPipelineData::createDescriptorLayouts(vireo);
    }

    void SceneRenderContext::destroyDescriptorLayouts() {
        sceneDescriptorLayout.reset();
        sceneDescriptorLayoutOptional1.reset();
        GraphicPipelineData::destroyDescriptorLayouts();
    }

    SceneRenderContext::SceneRenderContext(
        const Context& ctx,
        const SceneRenderContextConfiguration& config,
        const RendererConfiguration& renderingConfig,
        const uint32 framesInFlight,
        const vireo::Viewport& viewport,
        const vireo::Rect& scissors) :
        ctx(ctx),
        materialManager(ctx.res.get<MaterialManager>()),
        config{config},
        lightsBuffer{ctx.vireo->createBuffer(
            vireo::BufferType::UNIFORM,
            sizeof(LightData),
            1,
            "Scene Lights")},
        meshInstancesDataArray{ctx.vireo,
            sizeof(MeshInstanceData),
            config.maxMeshInstancesPerScene,
            config.maxMeshInstancesPerScene,
            vireo::BufferType::DEVICE_STORAGE,
            "meshInstances Data"},
        sceneUniformBuffer{ctx.vireo->createBuffer(
            vireo::BufferType::UNIFORM,
            sizeof(SceneData), 1,
            "Scene Data")},
        scissors{scissors},
        viewport{viewport},
        framesInFlight{framesInFlight},
        renderingConfig{renderingConfig} {

        const auto blankImage = ctx.res.get<ImageManager>().getBlankImage();
        shadowMaps.resize(config.maxShadowMaps * 6);
        shadowTransparencyColorMaps.resize(config.maxShadowMaps * 6);
        for (int i = 0; i < shadowMaps.size(); i++) {
            shadowMaps[i] = blankImage;
            shadowTransparencyColorMaps[i] = blankImage;
        }

        descriptorSet = ctx.vireo->createDescriptorSet(sceneDescriptorLayout, "Scene");
        descriptorSet->update(BINDING_SCENE, sceneUniformBuffer);
        descriptorSet->update(BINDING_MODELS, meshInstancesDataArray.getBuffer());
        descriptorSet->update(BINDING_LIGHTS, lightsBuffer);
        descriptorSet->update(BINDING_SHADOW_MAPS, shadowMaps);

        descriptorSetOpt1 = ctx.vireo->createDescriptorSet(sceneDescriptorLayoutOptional1, "Scene Opt1");
        descriptorSetOpt1->update(BINDING_SHADOW_MAP_TRANSPARENCY_COLOR, shadowTransparencyColorMaps);

        sceneUniformBuffer->map();
        lightsBuffer->map();
    }

    void SceneRenderContext::compute(const CameraDesc& camera, vireo::CommandList& commandList) const {
        compute(camera, commandList, opaquePipelinesData);
        compute(camera, commandList, shaderMaterialPipelinesData);
        compute(camera, commandList, transparentPipelinesData);
    }

    void SceneRenderContext::compute(
        const CameraDesc& camera,
        vireo::CommandList& commandList,
        const std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>>& pipelinesData) const {
        for (const auto& [pipelineId, pipelineData] : pipelinesData) {
            pipelineData->frustumCullingPipeline.dispatch(
                commandList,
                pipelineData->drawCommandsCount,
                camera.transform,
                camera.projection,
                *pipelineData->instancesArray.getBuffer(),
                *pipelineData->drawCommandsBuffer,
                *pipelineData->culledDrawCommandsBuffer,
                *pipelineData->culledDrawCommandsCountBuffer);
        }
        // for (const auto& renderer : std::views::values(shadowMapRenderers)) {
        //     const auto& shadowMapRenderer = std::static_pointer_cast<ShadowMapPass>(renderer);
        //     shadowMapRenderer->compute(commandList, pipelinesData);
        // }
    }

    void SceneRenderContext::updatePipelinesData(
        const vireo::CommandList& commandList,
        const std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>>& pipelinesData) {
        for (const auto& [pipelineId, pipelineData] : pipelinesData) {
            pipelineData->updateData(commandList, drawCommandsStagingBufferRecycleBin, meshInstancesDataMemoryBlocks);
        }
    }

    void SceneRenderContext::update(const CameraDesc& camera, const vireo::CommandList& commandList) {
        if (!drawCommandsStagingBufferRecycleBin.empty()) {
            drawCommandsStagingBufferRecycleBin.clear();
        }
        if (!removedLights.empty()) {
            for (const auto& light : removedLights) {
                lights.remove(light);
                //disableLightShadowCasting(light);
            }
        }
        if (!removedMeshInstances.empty()) {
            for (const auto& meshInstance : removedMeshInstances) {
                meshInstancesDataArray.free(meshInstancesDataMemoryBlocks.at(meshInstance));
                meshInstancesDataMemoryBlocks.erase(meshInstance);
            }
            meshInstancesDataUpdated = true;
            removedMeshInstances.clear();
        }

        if (shadowMapsUpdated) {
            descriptorSet->update(BINDING_SHADOW_MAPS, shadowMaps);
            descriptorSetOpt1->update(BINDING_SHADOW_MAP_TRANSPARENCY_COLOR, shadowTransparencyColorMaps);
            shadowMapsUpdated = false;
        }

        auto sceneUniform = SceneData {
            .cameraPosition = camera.position,
            .projection = camera.projection,
            .view = inverse(camera.transform),
            .viewInverse = camera.transform,
            .lightsCount = static_cast<uint32>(lights.size()),
            // .bloomEnabled = renderingConfig.bloomEnabled ? 1u : 0u,
            // .ssaoEnabled = renderingConfig.ssaoEnabled ? 1u : 0u,
        };
        if (environment) {
            sceneUniform.ambientLight = environment->ambientColorIntensity;
        }
        sceneUniformBuffer->write(&sceneUniform);

        for (const auto& meshInstance : std::views::keys(meshInstancesDataMemoryBlocks)) {
            if (meshInstance->pendingUpdates > 0) {
                const auto meshInstanceData = meshInstance->getData();
                meshInstancesDataArray.write(meshInstancesDataMemoryBlocks[meshInstance], &meshInstanceData);
                meshInstancesDataUpdated = true;
                meshInstance->pendingUpdates -= 1;
            }
        }

        if (meshInstancesDataUpdated) {
            meshInstancesDataArray.flush(commandList);
            meshInstancesDataArray.postBarrier(commandList);
            meshInstancesDataUpdated = false;
        }

        // const auto start = std::chrono::high_resolution_clock::now();
        updatePipelinesData(commandList, opaquePipelinesData);
        updatePipelinesData(commandList, shaderMaterialPipelinesData);
        updatePipelinesData(commandList, transparentPipelinesData);
        // const auto end = std::chrono::high_resolution_clock::now();
        // const std::chrono::duration<double, std::milli> duration = end - start;
        // if (duration.count() > 0.01) {
            // std::cout << "updatePipelinesData " << duration.count() << " ms\n";
        // }

        if (!lights.empty()) {
            if (lights.size() > lightsBufferCount) {
                if (lightsBufferCount >= config.maxLights) {
                    throw Exception("Too many lights");
                }
                lightsBufferCount = lights.size();
                lightsBuffer = ctx.vireo->createBuffer(
                    vireo::BufferType::UNIFORM,
                    sizeof(LightData) * lightsBufferCount, 1,
                    "Scene Lights");
                lightsBuffer->map();
                descriptorSet->update(BINDING_LIGHTS, lightsBuffer);
            }
            auto lightIndex = 0;
            auto lightsArray = std::vector<LightData>(lightsBufferCount);
            for (const auto& light : lights) {
                if (light->visible) {
                    lightsArray[lightIndex] = light->getData();
                    // if (shadowMapRenderers.contains(light)) {
                    //     const auto&shadowMapRenderer = std::static_pointer_cast<ShadowMapPass>(shadowMapRenderers[light]);
                    //     lightsArray[lightIndex].mapIndex = shadowMapIndex[light];
                    //     switch (light->getLightType()) {
                    //         case Light::LIGHT_DIRECTIONAL: {
                    //             for (int cascadeIndex = 0; cascadeIndex < lightsArray[lightIndex].cascadesCount ; cascadeIndex++) {
                    //                 lightsArray[lightIndex].lightSpace[cascadeIndex] =
                    //                     shadowMapRenderer->getLightSpace(cascadeIndex);
                    //                 lightsArray[lightIndex].cascadeSplitDepth[cascadeIndex] =
                    //                     shadowMapRenderer->getCascadeSplitDepth(cascadeIndex);
                    //             }
                    //             break;
                    //         }
                    //         case Light::LIGHT_SPOT: {
                    //             lightsArray[lightIndex].lightSpace[0] = shadowMapRenderer->getLightSpace(0);
                    //             break;
                    //         }
                    //         case Light::LIGHT_OMNI: {
                    //             break;
                    //         }
                    //         default:;
                    //     }
                    // }
                    lightIndex++;
                }
            }
            lightsBuffer->write(lightsArray.data(), lightsArray.size() * sizeof(LightData));
        }
    }

    void SceneRenderContext::addInstance(const std::shared_ptr<MeshInstanceDesc>& meshInstance) {
        if (meshInstancesDataMemoryBlocks.contains(meshInstance)) {
            return;
        }

        const auto& mesh = meshInstance->mesh;
        assert([&]{return !mesh.getMaterials().empty(); }, "Models without materials are not supported");
        if (!mesh.isUploaded()) {
            throw Exception("Mesh instance is not in VRAM");
        }

        meshInstancesDataMemoryBlocks[meshInstance] = meshInstancesDataArray.alloc(1);
        meshInstance->maxUpdates = framesInFlight;
        if (meshInstance->pendingUpdates == 0) {
            meshInstance->pendingUpdates = meshInstance->maxUpdates;
        }

        auto haveTransparentMaterial{false};
        auto haveShaderMaterial{false};
        auto nodePipelineIds = std::set<uint32>{};
        for (int i = 0; i < mesh.getSurfaces().size(); i++) {
            const auto& material = materialManager[meshInstance->getSurfaceMaterial(i)];
            haveTransparentMaterial = material.getTransparency() != Transparency::DISABLED;
            haveShaderMaterial = material.getType() == Material::SHADER;
            const auto id = material.getPipelineId();
            nodePipelineIds.insert(id);
            if (!pipelineIds.contains(id)) {
                pipelineIds[id].push_back(material.id);
                materialsUpdated = true;
            }
        }

        for (const auto& pipelineId : nodePipelineIds) {
            if (haveShaderMaterial) {
                addInstance(pipelineId, meshInstance, shaderMaterialPipelinesData);
            } else if (haveTransparentMaterial) {
                addInstance(pipelineId, meshInstance, transparentPipelinesData);
            } else {
                addInstance(pipelineId, meshInstance, opaquePipelinesData);
            }
        }
    }

    void SceneRenderContext::addInstance(
        pipeline_id pipelineId,
        const std::shared_ptr<MeshInstanceDesc>& meshInstance,
        std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>>& pipelinesData) {
        if (!pipelinesData.contains(pipelineId)) {
            pipelinesData[pipelineId] = std::make_unique<GraphicPipelineData>(
                ctx, config, pipelineId, meshInstancesDataArray);
        }
        pipelinesData[pipelineId]->addInstance(meshInstance, meshInstancesDataMemoryBlocks);
    }

    void SceneRenderContext::removeInstance(const std::shared_ptr<MeshInstanceDesc>& meshInstance) {
        if (!meshInstancesDataMemoryBlocks.contains(meshInstance)) {
            return;
        }
        for (const auto& pipelineId : std::views::keys(pipelineIds)) {
            if (shaderMaterialPipelinesData.contains(pipelineId)) {
                shaderMaterialPipelinesData[pipelineId]->removeInstance(meshInstance);
            }
            if (transparentPipelinesData.contains(pipelineId)) {
                transparentPipelinesData[pipelineId]->removeInstance(meshInstance);
            }
            if (opaquePipelinesData.contains(pipelineId)) {
                opaquePipelinesData[pipelineId]->removeInstance(meshInstance);
            }
        }
        removedMeshInstances.push_back(meshInstance);
    }

    void SceneRenderContext::drawOpaquesModels(
        vireo::CommandList& commandList,
        const std::unordered_map<uint32, std::shared_ptr<vireo::GraphicPipeline>>& pipelines) const {
        if (opaquePipelinesData.empty()) { return; }
        drawModels(commandList, pipelines, opaquePipelinesData);
    }

    void SceneRenderContext::drawTransparentModels(
        vireo::CommandList& commandList,
        const std::unordered_map<uint32, std::shared_ptr<vireo::GraphicPipeline>>& pipelines) const {
        if (transparentPipelinesData.empty()) { return; }
        drawModels(commandList, pipelines, transparentPipelinesData);
    }

    void SceneRenderContext::drawShaderMaterialModels(
        vireo::CommandList& commandList,
        const std::unordered_map<uint32, std::shared_ptr<vireo::GraphicPipeline>>& pipelines) const {
        if (shaderMaterialPipelinesData.empty()) { return; }
        drawModels(commandList, pipelines, shaderMaterialPipelinesData);
    }

    void SceneRenderContext::setInitialState(const vireo::CommandList& commandList) const {
        commandList.setViewport(viewport);
        commandList.setScissors(scissors);
    }

    void SceneRenderContext::drawModels(
        vireo::CommandList& commandList,
        const uint32 set,
        const std::map<pipeline_id, std::shared_ptr<vireo::Buffer>>& culledDrawCommandsBuffers,
        const std::map<pipeline_id, std::shared_ptr<vireo::Buffer>>& culledDrawCommandsCountBuffers,
        const std::map<pipeline_id, std::shared_ptr<FrustumCulling>>& frustumCullingPipelines) const {
        for (const auto& [pipelineId, pipelineData] : opaquePipelinesData) {
            if (pipelineData->drawCommandsCount == 0 ||
                frustumCullingPipelines.at(pipelineId)->getDrawCommandsCount() == 0) { continue; }
            commandList.bindDescriptor(pipelineData->descriptorSet, set);
            // commandList.drawIndexedIndirect(
                // pipelineData->drawCommandsBuffer,
                // 0,
                // pipelineData->drawCommandsCount,
                // sizeof(DrawCommand),
                // sizeof(uint32));
            commandList.drawIndexedIndirectCount(
                culledDrawCommandsBuffers.at(pipelineId),
                0,
                culledDrawCommandsCountBuffers.at(pipelineId),
                0,
                pipelineData->drawCommandsCount,
                sizeof(DrawCommand),
                sizeof(uint32));
        }
        for (const auto& [pipelineId, pipelineData] : shaderMaterialPipelinesData) {
            if (pipelineData->drawCommandsCount == 0 ||
                frustumCullingPipelines.at(pipelineId)->getDrawCommandsCount() == 0) { continue; }
            commandList.bindDescriptor(pipelineData->descriptorSet, set);
            commandList.drawIndexedIndirectCount(
                culledDrawCommandsBuffers.at(pipelineId),
                0,
                culledDrawCommandsCountBuffers.at(pipelineId),
                0,
                pipelineData->drawCommandsCount,
                sizeof(DrawCommand),
                sizeof(uint32));
        }
        for (const auto& [pipelineId, pipelineData] : transparentPipelinesData) {
            if (pipelineData->drawCommandsCount == 0 ||
                frustumCullingPipelines.at(pipelineId)->getDrawCommandsCount() == 0) { continue; }
            commandList.bindDescriptor(pipelineData->descriptorSet, set);
            commandList.drawIndexedIndirectCount(
                culledDrawCommandsBuffers.at(pipelineId),
                0,
                culledDrawCommandsCountBuffers.at(pipelineId),
                0,
                pipelineData->drawCommandsCount,
                sizeof(DrawCommand),
                sizeof(uint32));
        }
    }

    void SceneRenderContext::drawModels(
        vireo::CommandList& commandList,
        const std::unordered_map<uint32, std::shared_ptr<vireo::GraphicPipeline>>& pipelines,
        const std::unordered_map<uint32, std::unique_ptr<GraphicPipelineData>>& pipelinesData) const {
        for (const auto& [pipelineId, pipelineData] : pipelinesData) {
            if (pipelineData->drawCommandsCount == 0 ||
                pipelineData->frustumCullingPipeline.getDrawCommandsCount() == 0) { continue; }
            const auto& pipeline = pipelines.at(pipelineId);
            commandList.bindPipeline(pipeline);
                commandList.bindDescriptors({
                    ctx.descriptorSet,
                    ctx.samplers.getDescriptorSet(),
                    descriptorSet,
                    pipelineData->descriptorSet,
                    descriptorSetOpt1,
                });

            commandList.drawIndexedIndirectCount(
                pipelineData->culledDrawCommandsBuffer,
                0,
                pipelineData->culledDrawCommandsCountBuffer,
                0,
                pipelineData->drawCommandsCount,
                sizeof(DrawCommand),
                sizeof(uint32));
        }
    }

    // void SceneRenderContext::enableLightShadowCasting(const std::shared_ptr<Node>&node) {
    //     if (const auto& light = std::dynamic_pointer_cast<Light>(node)) {
    //         if (light->getCastShadows() && !shadowMapRenderers.contains(light) && (shadowMapRenderers.size() < MAX_SHADOW_MAPS)) {
    //             const auto shadowMapRenderer = make_shared<ShadowMapPass>(
    //                 config,
    //                 renderingConfig,
    //                 light,
    //                 meshInstancesDataArray);
    //             // INFO("enableLightShadowCasting for ", std::to_string(light->getName()));
    //             materialsUpdated = true; // force update pipelines
    //             shadowMapRenderers[light] = shadowMapRenderer;
    //             shadowMapRenderer->setCurrentCamera(currentCamera);
    //             const auto& blankImage = Application::getResources().getBlankImage();
    //             for (uint32 index = 0; index < shadowMaps.size(); index += 6) {
    //                 if (shadowMaps[index] == blankImage) {
    //                     shadowMapIndex[light] = index;
    //                     for (int i = 0; i < shadowMapRenderer->getShadowMapCount(); i++) {
    //                         shadowMaps[index + i] = shadowMapRenderer->getShadowMap(i)->getImage();
    //                         shadowTransparencyColorMaps[index + i] = shadowMapRenderer->getTransparencyColorMap(i)->getImage();
    //                     }
    //                     shadowMapsUpdated = true;
    //                     return;
    //                 }
    //             }
    //             throw Exception("Out of memory for shadow map");
    //         }
    //     }
    // }

    // void SceneRenderContext::disableLightShadowCasting(const std::shared_ptr<Light>&light) {
    //     if (shadowMapRenderers.contains(light)) {
    //         // INFO("disableLightShadowCasting for ", std::to_string(light->getName()));
    //         const auto& shadowMapRenderer = std::static_pointer_cast<ShadowMapPass>(shadowMapRenderers.at(light));
    //         const auto index = shadowMapIndex[light];
    //         const auto& blankImage = Application::getResources().getBlankImage();
    //         for (int i = 0; i < shadowMapRenderer->getShadowMapCount(); i++) {
    //             shadowMaps[index + i] = blankImage;
    //             shadowTransparencyColorMaps[index + i] = blankImage;
    //         }
    //         shadowMapsUpdated = true;
    //         shadowMapIndex.erase(light);
    //         shadowMapRenderers.erase(light);
    //     }
    // }

}
