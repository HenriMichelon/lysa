/*
* Copyright (c) 2025-present Henri Michelon
*
* This software is released under the MIT License.
* https://opensource.org/licenses/MIT
*/
module;
extern "C"
{
    #define LUA_LIB
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
    int luaopen_socket_core(lua_State* L);
}
module lysa.lua;

import vireo.lua;
import lysa;

template <> struct luabridge::Stack<lysa::LogLevel> : Enum<lysa::LogLevel> {};
template <> struct luabridge::Stack<lysa::LoggingMode> : Enum<lysa::LoggingMode> {};
template <> struct luabridge::Stack<lysa::RendererType> : Enum<lysa::RendererType> {};
template <> struct luabridge::Stack<lysa::RenderingWindowMode> : Enum<lysa::RenderingWindowMode> {};

namespace lysa {

    Lua::Lua(const LuaConfiguration& luaConfiguration, const VirtualFS& virtualFs) : virtualFs(virtualFs) {
        L = luaL_newstate();
        luaL_openlibs(L);
        luaL_requiref(L, "socket", luaopen_socket_core, 1);
        lua_pop(L, 1);
        luaL_requiref(L, "socket.core", luaopen_socket_core, 1);
        lua_pop(L, 1);
        luabridge::enableExceptions(L);
        bind();

        const std::string path_chunk = "package.path = package.path .. ';?.lua;" + virtualFs.getScriptsDirectory()  + "/?.lua'";
        if (luaL_dostring(L, path_chunk.c_str()) != LUA_OK) {
            Log::warning("[Lua] error: ", std::string(lua_tostring(L, -1)));
            lua_pop(L, 1);
        }
        if (luaConfiguration.startRemoteDebug) {
             std::string mobdebug_chunk = R"(
local ok, mobdebug = pcall(require, 'mobdebug')
if not ok then
    error('mobdebug not found: ' .. tostring(mobdebug))
end
)";
            mobdebug_chunk +=
                "mobdebug.start([[" + luaConfiguration.remoteDebugHosts + "]], " +
                std::to_string(luaConfiguration.remoteDebugPort) + ")\n";
            if (luaL_dostring(L, mobdebug_chunk.c_str()) != LUA_OK) {
                Log::warning("[Lua/mobdebug] error: ", std::string(lua_tostring(L, -1)));
                lua_pop(L, 1);
            }
        }
    }

    Lua::~Lua() {
        lua_close(L);
    }

    luabridge::Namespace Lua::beginNamespace() const {
        return luabridge::getGlobalNamespace(L).beginNamespace ("lysa");
    }

    luabridge::Namespace Lua::beginNamespace(const std::string& name) const {
        return luabridge::getGlobalNamespace(L).beginNamespace (name.c_str());
    }

    luabridge::LuaRef Lua::getGlobal(const std::string & name) const {
        return luabridge::getGlobal(L, name.c_str());
    }

    void Lua::execute(const std::string& scriptName) const{
        std::vector<char> data;
        virtualFs.loadScript(scriptName, data);
        const auto script = std::string(data.begin(), data.end());
        if (script.empty()) {
            throw Exception("Lua error: failed to load script");
        }

        if (luaL_dostring(L, script.c_str()) != LUA_OK) {
            const char* err = lua_tostring(L, -1);
            std::string msg = err ? err : "(unknown error)";
            lua_pop(L, 1);
            throw Exception("Lua error : ", msg);
        }
    }

    void Lua::bind() {
        beginNamespace("std")
            .beginClass<std::string>("string")
                .addFunction("append",
                    static_cast<std::string& (std::string::*)(const char*)>(
                           &std::string::append
                       )
                    )
            .endClass()
            .beginClass<std::u32string>("u32string")
            .endClass()
        .endNamespace();

        vireo::LuaBindings::_register(L);

        beginNamespace()
        .beginClass<float2>("float2")
           .addConstructor<void(float, float)>()
           .addProperty("x", &float2::x)
           .addProperty("r", &float2::r)
           .addProperty("y", &float2::y)
           .addProperty("g", &float2::g)
       .endClass()
       .beginClass<float3>("float3")
           .addConstructor<void(float, float, float)>()
           .addProperty("x", &float3::x)
           .addProperty("r", &float3::r)
           .addProperty("y", &float3::y)
           .addProperty("g", &float3::g)
           .addProperty("z", &float3::z)
           .addProperty("b", &float3::b)
       .endClass()
       .beginClass<float4>("float4")
           .addConstructor<void(float, float, float, float)>()
           .addProperty("x", &float4::x)
           .addProperty("r", &float4::r)
           .addProperty("y", &float4::y)
           .addProperty("g", &float4::g)
           .addProperty("z", &float4::z)
           .addProperty("b", &float4::b)
           .addProperty("w", &float4::w)
           .addProperty("a", &float4::a)
       .endClass()
       .beginClass<quaternion>("quaternion")
           .addConstructor<void(float, float, float, float)>()
           .addProperty("x", &quaternion::x)
           .addProperty("r", &quaternion::r)
           .addProperty("y", &quaternion::y)
           .addProperty("g", &quaternion::g)
           .addProperty("z", &quaternion::z)
           .addProperty("b", &quaternion::b)
           .addProperty("w", &quaternion::w)
           .addProperty("a", &quaternion::a)
        .endClass()
        .addFunction("euler_angles", &euler_angles)
        .addFunction("radians", luabridge::overload<float>(radians))
        .addFunction("almost_equals",
            luabridge::overload<float, float>(almost_equals),
            luabridge::overload<const quaternion&, const quaternion&>(almost_equals)
        )
        .addFunction("look_at", &look_at)
        .addFunction("perspective", &perspective)
        .addFunction("orthographic", &orthographic)
        .addFunction("randomi", &randomi)
        .addFunction("randomf", &randomf)

        .addFunction("is_windows", +[]{ return is_windows(); })
        .addFunction("get_current_time_milliseconds", get_current_time_milliseconds)
        .addFunction("sanitize_name", sanitize_name)
        .addFunction("dir_exists", dir_exists)
        .addFunction("to_float3", to_float3)
        .addFunction("to_float4", to_float4)
        .addFunction("to_lower", to_lower)

        .beginClass<Log>("Log")
           .addStaticFunction("log", +[](const char*msg) { Log::log(msg); })
           .addStaticFunction("debug", +[](const char*msg) { Log::debug(msg); })
           .addStaticFunction("info", +[](const char*msg) { Log::info(msg); })
           .addStaticFunction("game1", +[](const char* msg) { Log::game1(msg); })
           .addStaticFunction("game2", +[](const char*msg) { Log::game2(msg); })
           .addStaticFunction("game3", +[](const char*msg) { Log::game3(msg); })
           .addStaticFunction("warning", +[](const char*msg) { Log::warning(msg); })
           .addStaticFunction("error", +[](const char*msg) { Log::error(msg); })
           .addStaticFunction("critical", +[](const char*msg) { Log::critical(msg); })
        .endClass()

        .beginClass<Event>("Event")
            .addProperty("id", &Event::id)
            .addProperty("type", &Event::type)
        .endClass()
        .beginClass<EventManager>("EventManager")
            .addFunction("push", &EventManager::push)
            .addFunction("subscribe",
                luabridge::overload<const event_type&, unique_id, luabridge::LuaRef>(&EventManager::subscribe))
        .endClass()
        .beginClass<VirtualFS>("VirtualFS")
            .addFunction("get_path", &VirtualFS::getPath)
            .addFunction("file_exists", &VirtualFS::fileExists)
        .endClass()

        .beginNamespace("RenderingWindowMode")
            .addVariable("WINDOWED", RenderingWindowMode::WINDOWED)
            .addVariable("WINDOWED_MAXIMIZED", RenderingWindowMode::WINDOWED_MAXIMIZED)
            .addVariable("WINDOWED_FULLSCREEN", RenderingWindowMode::WINDOWED_FULLSCREEN)
            .addVariable("FULLSCREEN", RenderingWindowMode::FULLSCREEN)
        .endNamespace()
        .beginNamespace("RenderingWindowEventType")
            .addVariable("READY", &RenderingWindowEvent::READY)
            .addVariable("CLOSING", &RenderingWindowEvent::CLOSING)
            .addVariable("RESIZED", &RenderingWindowEvent::RESIZED)
        .endNamespace()
        .beginClass<RenderingWindowEvent>("RenderingWindowEvent")
            .addProperty("id", &RenderingWindowEvent::id)
            .addProperty("type", &RenderingWindowEvent::type)
        .endClass()
        .beginClass<RenderingWindowConfiguration>("RenderingWindowConfiguration")
            .addConstructor<void()>()
            .addProperty("title", &RenderingWindowConfiguration::title)
            .addProperty("mode", &RenderingWindowConfiguration::mode)
            .addProperty("x", &RenderingWindowConfiguration::x)
            .addProperty("y", &RenderingWindowConfiguration::y)
            .addProperty("width", &RenderingWindowConfiguration::width)
            .addProperty("height", &RenderingWindowConfiguration::height)
            .addProperty("monitor", &RenderingWindowConfiguration::monitor)
        .endClass()
        .beginClass<RenderingWindow>("RenderingWindow")
           .addProperty("id", &RenderingWindow::id)
           .addProperty("x", &RenderingWindow::getX)
           .addProperty("y", &RenderingWindow::getY)
           .addProperty("width", &RenderingWindow::getWidth)
           .addProperty("height", &RenderingWindow::getHeight)
           .addProperty("stopped", &RenderingWindow::isStopped)
           .addProperty("platform_handle", &RenderingWindow::getPlatformHandle)
            .addFunction("show", &RenderingWindow::show)
            .addFunction("close", &RenderingWindow::close)
        .endClass()
        .beginClass<RenderingWindowManager>("RenderingWindowManager")
           .addFunction("create", &RenderingWindowManager::create)
           .addFunction("get",
               luabridge::nonConstOverload<const unique_id>(&RenderingWindowManager::get),
               luabridge::constOverload<const unique_id>(&RenderingWindowManager::get)
               )
        .endClass()

        .beginClass<RenderTargetConfiguration>("RenderTargetConfiguration")
            .addConstructor<void()>()
            .addProperty("rendering_window_handle", &RenderTargetConfiguration::renderingWindowHandle)
            .addProperty("swap_chain_format", &RenderTargetConfiguration::swapChainFormat)
            .addProperty("present_mode", &RenderTargetConfiguration::presentMode)
            .addProperty("renderer_configuration", &RenderTargetConfiguration::rendererConfiguration)
        .endClass()
        .beginNamespace("RenderTargetEventType")
            .addVariable("PAUSED", &RenderTargetEvent::PAUSED)
            .addVariable("RESUMED", &RenderTargetEvent::RESUMED)
            .addVariable("RESIZED", &RenderTargetEvent::RESIZED)
        .endNamespace()
        .beginClass<RenderTargetEvent>("RenderTargetEvent")
            .addProperty("id", &RenderTargetEvent::id)
            .addProperty("type", &RenderTargetEvent::type)
        .endClass()
        .beginClass<RenderTarget>("RenderTarget")
           .addProperty("id", &RenderTarget::id)
           .addProperty("pause", &RenderTarget::isPaused, &RenderTarget::pause)
           .addProperty("swap_chain", &RenderTarget::getSwapChain)
           .addProperty("rendering_window_handle", &RenderTarget::getRenderingWindowHandle)
           .addProperty("aspect_ratio", &RenderTarget::getAspectRatio)
        .endClass()
        .beginClass<RenderTargetManager>("RenderTargetManager")
            .addFunction("create", +[](RenderTargetManager* self, const RenderTargetConfiguration& config) -> RenderTarget& {
                return self->create(config);
            })
            .addFunction("get",
                luabridge::nonConstOverload<const unique_id>(&RenderTargetManager::get),
                luabridge::constOverload<const unique_id>(&RenderTargetManager::get)
                )
            .addFunction("destroy",
               luabridge::overload<const unique_id>(&Manager<RenderTarget>::destroy),
               luabridge::overload<const void*>(&RenderTargetManager::destroy)
            )
        .endClass()

        .beginClass<Samplers>("Samplers")
            .addStaticProperty("NEAREST_NEAREST_CLAMP_TO_BORDER", &Samplers::NEAREST_NEAREST_CLAMP_TO_BORDER)
            .addStaticProperty("LINEAR_LINEAR_CLAMP_TO_EDGE", &Samplers::LINEAR_LINEAR_CLAMP_TO_EDGE)
            .addStaticProperty("LINEAR_LINEAR_CLAMP_TO_EDGE_LOD_CLAMP_NONE", &Samplers::LINEAR_LINEAR_CLAMP_TO_EDGE_LOD_CLAMP_NONE)
            .addStaticProperty("LINEAR_LINEAR_REPEAT", &Samplers::LINEAR_LINEAR_REPEAT)
            .addStaticProperty("NEAREST_NEAREST_REPEAT", &Samplers::NEAREST_NEAREST_REPEAT)
        .endClass()

        .beginClass<Image>("Image")
            .addProperty("id", &Image::id)
            .addProperty("width", &Image::getWidth)
            .addProperty("height", &Image::getHeight)
            .addProperty("size", &Image::getSize)
            .addProperty("name", &Image::getName)
            .addProperty("index", &Image::getIndex)
            .addProperty("image", &Image::getImage)
        .endClass()
        .beginClass<ImageManager>("ImageManager")
            .addFunction("load", +[](ImageManager* self, const std::string& path) -> Image& {
                return self->load(path);
            })
            .addFunction("save", &ImageManager::save)
            .addProperty("blank_image", &ImageManager::getBlankImage)
            .addProperty("blank_cube_map", &ImageManager::getBlankCubeMap)
            .addProperty("images", &ImageManager::getImages)
            .addFunction("get",
               luabridge::nonConstOverload<const unique_id>(&ImageManager::get),
               luabridge::constOverload<const unique_id>(&ImageManager::get)
               )
            .addFunction("destroy",
              luabridge::overload<const unique_id>(&Manager<Image>::destroy)
           )
        .endClass()

        .beginClass<Texture>("Texture")
            .addProperty("id", &Texture::id)
            .addProperty("width", &Texture::getWidth)
            .addProperty("height", &Texture::getHeight)
            .addProperty("size", &Texture::getSize)
            .addProperty("name", &Texture::getName)
        .endClass()
        .beginClass<ImageTexture>("ImageTexture")
            .addProperty("id", &ImageTexture::id)
            .addProperty("width", &ImageTexture::getWidth)
            .addProperty("height", &ImageTexture::getHeight)
            // .addProperty("size", &ImageTexture::getSize)
            .addProperty("image", &ImageTexture::getImage)
            .addProperty("sampler_index", &ImageTexture::getSamplerIndex)
        .endClass()
        .beginClass<ImageTextureManager>("ImageTextureManager")
            .addFunction("create", +[](ImageTextureManager* self, const Image& image, unique_id sampler) -> ImageTexture& {
                    return self->create(image, sampler);
            })
           .addFunction("get",
              luabridge::nonConstOverload<const unique_id>(&ImageTextureManager::get),
              luabridge::constOverload<const unique_id>(&ImageTextureManager::get)
              )
           .addFunction("destroy",
             luabridge::overload<const unique_id>(&Manager<ImageTexture>::destroy)
          )
       .endClass()

        .beginNamespace("Transparency")
            .addVariable("DISABLED", Transparency::DISABLED)
            .addVariable("ALPHA", Transparency::ALPHA)
        .endNamespace()
        .beginNamespace("MaterialType")
            .addVariable("STANDARD", Material::Type::STANDARD)
            .addVariable("SHADER", Material::Type::SHADER)
        .endNamespace()
        .beginClass<Material>("Material")
            .addProperty("id", &Material::id)
            .addProperty("cull_mode", &Material::getCullMode, &Material::setCullMode)
            .addProperty("transparency", &Material::getTransparency, &Material::setTransparency)
            .addProperty("alpha_scissor", &Material::getAlphaScissor, &Material::setAlphaScissor)
            .addProperty("index", &Material::getIndex)
            .addProperty("type", &Material::getType)
        .endClass()
        .beginClass<StandardMaterial::TextureInfo>("TextureInfo")
            .addConstructor<void(const ImageTexture*)>()
            .addProperty("texture", &StandardMaterial::TextureInfo::texture)
            .addProperty("transform", &StandardMaterial::TextureInfo::transform)
        .endClass()
        .beginClass<StandardMaterial>("StandardMaterial")
            .addProperty("id", &StandardMaterial::id)
            .addProperty("cull_mode",
                +[](const StandardMaterial* self) -> vireo::CullMode {
                    return self->getCullMode();
                },
                +[](StandardMaterial* self, vireo::CullMode cullmode) {
                    return self->setCullMode(cullmode);
                }
            )
            .addProperty("transparency",
                +[](const StandardMaterial* self) -> Transparency {
                    return self->getTransparency();
                },
                +[](StandardMaterial* self, Transparency t) {
                    return self->setTransparency(t);
                }
            )
            .addProperty("albedo_color", &StandardMaterial::getAlbedoColor, &StandardMaterial::setAlbedoColor)
            .addProperty("diffuse_texture", &StandardMaterial::getDiffuseTexture, &StandardMaterial::setDiffuseTexture)
            .addProperty("normal_texture", &StandardMaterial::getNormalTexture, &StandardMaterial::setNormalTexture)
            .addProperty("metallic_factor", &StandardMaterial::getMetallicFactor, &StandardMaterial::setMetallicFactor)
            .addProperty("metallic_texture", &StandardMaterial::getMetallicTexture, &StandardMaterial::setMetallicTexture)
            .addProperty("roughness_factor", &StandardMaterial::getRoughnessFactor, &StandardMaterial::setRoughnessFactor)
            .addProperty("roughness_texture", &StandardMaterial::getRoughnessTexture, &StandardMaterial::setRoughnessTexture)
            .addProperty("emissive_texture", &StandardMaterial::getEmissiveTexture, &StandardMaterial::setEmissiveTexture)
            .addProperty("emissive_factor", &StandardMaterial::getEmissiveFactor, &StandardMaterial::setEmissiveFactor)
            .addProperty("emissive_strength", &StandardMaterial::getEmissiveStrength, &StandardMaterial::setEmissiveStrength)
            .addProperty("emissive_texture", &StandardMaterial::getEmissiveTexture, &StandardMaterial::setEmissiveTexture)
            .addProperty("normal_scale", &StandardMaterial::getNormalScale, &StandardMaterial::setNormalScale)
        .endClass()
        .beginClass<ShaderMaterial>("ShaderMaterial")
            .addProperty("id", &ShaderMaterial::id)
            .addProperty("cull_mode",
                +[](const ShaderMaterial* self) -> vireo::CullMode {
                    return self->getCullMode();
                },
                +[](ShaderMaterial* self, vireo::CullMode cullmode) {
                    return self->setCullMode(cullmode);
                }
            )
            .addProperty("transparency",
                +[](const ShaderMaterial* self) -> Transparency {
                    return self->getTransparency();
                },
                +[](ShaderMaterial* self, Transparency t) {
                    return self->setTransparency(t);
                }
            )
            .addProperty("frag_file_name", &ShaderMaterial::getFragFileName)
            .addProperty("vert_file_name", &ShaderMaterial::getVertFileName)
            .addFunction("set_parameter", &ShaderMaterial::setParameter)
            .addFunction("get_parameter", &ShaderMaterial::getParameter)
        .endClass()
        .beginClass<MaterialManager>("MaterialManager")
               .addFunction("create_standard", +[](MaterialManager* self) -> StandardMaterial& {
                        return self->create();
                })
                .addFunction("create_shared", +[](MaterialManager* self, const std::string &f, const std::string &v) -> ShaderMaterial& {
                         return self->create(f, v);
                 })
               .addFunction("get",
                 luabridge::nonConstOverload<const unique_id>(&MaterialManager::get),
                 luabridge::constOverload<const unique_id>(&MaterialManager::get)
                 )
        .endClass()

        .beginClass<AABB>("AABB")
            .addProperty("min", &AABB::min)
            .addProperty("max", &AABB::max)
        .endClass()

        .beginClass<Vertex>("Vertex")
            .addConstructor<void(float3, float3, float2, float4)>()
            .addProperty("position", &Vertex::position)
            .addProperty("normal", &Vertex::normal)
            .addProperty("uv", &Vertex::uv)
            .addProperty("tangent", &Vertex::tangent)
        .endClass()
        .beginClass<MeshSurface>("MeshSurface")
            .addConstructor<void(uint32, uint32)>()
            .addProperty("firstIndex", &MeshSurface::firstIndex)
            .addProperty("indexCount", &MeshSurface::indexCount)
            .addProperty("material", &MeshSurface::material)
        .endClass()
        .beginClass<Mesh>("Mesh")
            .addProperty("id", &Mesh::id)
            .addFunction("get_surface_material", &Mesh::getSurfaceMaterial)
            .addFunction("set_surface_material", &Mesh::setSurfaceMaterial)
            .addFunction("aabb", &Mesh::getAABB)
        .endClass()
        .beginClass<MeshManager>("MeshManager")
           // .addFunction("create", +[](MeshManager* self) -> Mesh& {
           //          return self->create();
           //  })
            .addFunction("create", +[](MeshManager* self, const luabridge::LuaRef& v, const luabridge::LuaRef&i, const luabridge::LuaRef&s) -> Mesh& {
                     return self->create(v, i, s);
             })
           .addFunction("get",
             luabridge::nonConstOverload<const unique_id>(&MeshManager::get),
             luabridge::constOverload<const unique_id>(&MeshManager::get)
             )
        .endClass()

        .beginClass<Scene>("Scene")
            .addProperty("id", &Scene::id)
            .addProperty("ambientLight", &Scene::getAmbientLight, &Scene::setAmbientLight)
        .endClass()
        .beginClass<SceneManager>("SceneManager")
           .addFunction("create", +[](SceneManager* self) -> Scene& {
                    return self->create();
            })
           .addFunction("get",
             luabridge::nonConstOverload<const unique_id>(&SceneManager::get),
             luabridge::constOverload<const unique_id>(&SceneManager::get)
             )
        .endClass()

        .beginClass<Renderpass>("Renderpass")
        .endClass()

        .beginNamespace("RendererType")
            .addVariable("FORWARD", RendererType::FORWARD)
            .addVariable("DEFERRED", RendererType::DEFERRED)
        .endNamespace()
        .beginClass<RendererConfiguration>("RendererConfiguration")
            .addConstructor<void()>()
            .addProperty("renderer_type", &RendererConfiguration::rendererType)
            .addProperty("color_rendering_format", &RendererConfiguration::colorRenderingFormat)
            .addProperty("depth_stencil_format", &RendererConfiguration::depthStencilFormat)
            .addProperty("clear_color", &RendererConfiguration::clearColor)
            .addProperty("msaa", &RendererConfiguration::msaa)
        .endClass()
        .beginClass<Renderer>("Renderer")
        .endClass()

        .beginClass<ResourcesRegistry>("ResourcesRegistry")
            .addProperty("render_target_manager",
                +[](const ResourcesRegistry* rl) -> RenderTargetManager& {
                    return rl->get<RenderTargetManager>();
            })
            .addProperty("rendering_window_manager",
                +[](const ResourcesRegistry* rl) -> RenderingWindowManager& {
                return rl->get<RenderingWindowManager>();
            })
            .addProperty("image_manager",
                +[](const ResourcesRegistry* rl) -> ImageManager& {
                return rl->get<ImageManager>();
            })
            .addProperty("image_texture_manager",
                +[](const ResourcesRegistry* rl) -> ImageTextureManager& {
                return rl->get<ImageTextureManager>();
            })
            .addProperty("material_manager",
                +[](const ResourcesRegistry* rl) -> MaterialManager& {
                return rl->get<MaterialManager>();
            })
            .addProperty("mesh_manager",
                +[](const ResourcesRegistry* rl) -> MeshManager& {
                return rl->get<MeshManager>();
            })
            .addProperty("scene_manager",
                +[](const ResourcesRegistry* rl) -> SceneManager& {
                return rl->get<SceneManager>();
            })
        .endClass()
        .endNamespace();

#ifdef ECS_SCENES
        beginNamespace("ecs")
            .beginClass<ecs::RenderTarget>("RenderTarget")
                .addConstructor<void(unique_id)>()
                .addProperty("render_target", &ecs::RenderTarget::renderTarget)
            .endClass()
            .beginClass<ecs::Viewport>("Viewport")
                .addConstructor<void(vireo::Viewport, vireo::Rect)>()
                .addProperty("viewport", &ecs::Viewport::viewport)
                .addProperty("scissors", &ecs::Viewport::scissors)
            .endClass()
            .beginClass<ecs::Camera>("Camera")
                .addConstructor<void()>()
                .addProperty("is_perspective", &ecs::Camera::isPerspective)
                .addProperty("fov", &ecs::Camera::fov)
                .addProperty("aspect_ratio", &ecs::Camera::aspectRatio)
                .addProperty("near", &ecs::Camera::near)
                .addProperty("far", &ecs::Camera::far)
                .addProperty("left", &ecs::Camera::left)
                .addProperty("right", &ecs::Camera::right)
                .addProperty("top", &ecs::Camera::top)
                .addProperty("bottom", &ecs::Camera::bottom)
            .endClass()
            .beginClass<ecs::CameraRef>("CameraRef")
                .addProperty("camera", &ecs::CameraRef::camera)
            .endClass()
            .beginClass<ecs::MaterialOverride>("MaterialOverride")
                .addProperty("surface_index", &ecs::MaterialOverride::surfaceIndex)
                .addProperty("material", &ecs::MaterialOverride::material)
            .endClass()
            .beginClass<ecs::MeshInstance>("MeshInstance")
                .addProperty("mesh", &ecs::MeshInstance::mesh)
            .endClass()
            .beginClass<ecs::Scene>("Scene")
                .addConstructor<void(unique_id)>()
                .addProperty("scene", &ecs::Scene::scene)
            .endClass()
            .beginClass<ecs::SceneRef>("SceneRef")
                .addProperty("scene", &ecs::SceneRef::scene)
            .endClass()
            .beginClass<ecs::AmbientLight>("AmbientLight")
                .addConstructor<void()>()
                .addProperty("color", &ecs::AmbientLight::color)
                .addProperty("intensity", &ecs::AmbientLight::intensity)
            .endClass()
            .beginClass<ecs::Visible>("Visible").endClass()
            .beginClass<ecs::Transform>("Transform")
                .addConstructor<void()>()
                .addProperty("local", &ecs::Transform::local)
                .addProperty("global", &ecs::Transform::global)
            .endClass()

            .addFunction("set_position",
                luabridge::overload<flecs::entity, const float3&>(&ecs::setPosition),
                luabridge::overload<flecs::entity, float, float, float>(&ecs::setPosition)
            )

            .beginClass<flecs::world>("world")
                .addFunction("entity", +[](const flecs::world* w) { return w->entity<>(); })
            .endClass()

            .beginClass<flecs::entity>("entity")
                .addProperty("is_alive", +[](const flecs::entity* e) { return e->is_alive(); })
                .addFunction("destruct", &flecs::entity::destruct)
                .addProperty("render_target",
                    [](const flecs::entity* e) { return e->get_mut<ecs::RenderTarget>(); },
                    [](const flecs::entity* e, const ecs::RenderTarget& c) { e->set<ecs::RenderTarget>(c);})
                .addProperty("viewport",
                    [](const flecs::entity* e) { return e->get_mut<ecs::Viewport>(); },
                    [](const flecs::entity* e, const ecs::Viewport& c) { e->set<ecs::Viewport>(c);})
                .addProperty("camera",
                    [](const flecs::entity* e) { return e->get_mut<ecs::Camera>(); },
                    [](const flecs::entity* e, const ecs::Camera& c) { e->set<ecs::Camera>(c);})
                .addProperty("camera_ref",
                    [](const flecs::entity* e) { return e->get_mut<ecs::CameraRef>(); },
                    [](const flecs::entity* e, const ecs::CameraRef& c) { e->set<ecs::CameraRef>(c);})
                .addProperty("material_override",
                    [](const flecs::entity* e) { return e->get_mut<ecs::MaterialOverride>(); },
                    [](const flecs::entity* e, const ecs::MaterialOverride& c) { e->set<ecs::MaterialOverride>(c);})
                .addProperty("mesh_instance",
                    [](const flecs::entity* e) { return e->get_mut<ecs::MeshInstance>(); },
                    [](const flecs::entity* e, const ecs::MeshInstance& c) { e->set<ecs::MeshInstance>(c);})
                .addProperty("scene",
                    [](const flecs::entity* e) { return e->get_mut<ecs::Scene>(); },
                    [](const flecs::entity* e, const ecs::Scene& c) { e->set<ecs::Scene>(c);})
                .addProperty("scene_ref",
                    [](const flecs::entity* e) { return e->get_mut<ecs::SceneRef>(); },
                    [](const flecs::entity* e, const ecs::SceneRef& c) { e->set<ecs::SceneRef>(c);})
                .addProperty("ambient_light",
                    [](const flecs::entity* e) { return e->get_mut<ecs::AmbientLight>(); },
                    [](const flecs::entity* e, const ecs::AmbientLight& c) { e->set<ecs::AmbientLight>(c);})
                .addProperty("visible",
                    [](const flecs::entity* e) { return e->get_mut<ecs::Visible>(); },
                    [](const flecs::entity* e, const ecs::Visible& c) { e->set<ecs::Visible>(c);})
                .addProperty("transform",
                    [](const flecs::entity* e) { return e->get_mut<ecs::Transform>(); },
                    [](const flecs::entity* e, const ecs::Transform& c) { e->set<ecs::Transform>(c);})
            .endClass()
        .endNamespace();
#endif
    }
}