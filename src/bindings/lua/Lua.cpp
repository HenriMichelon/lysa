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

import vireo;
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

        if (luaConfiguration.startRemoteDebug) {
             std::string mobdebug_chunk = R"(
package.path = package.path .. ";./scripts/?.lua;./?.lua"
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
            .endClass()
            .beginClass<std::u32string>("u32string")
            .endClass()
        .endNamespace();

        beginNamespace("flecs")
            .beginClass<flecs::world>("world")
                .addFunction("entity", +[](const flecs::world* w) { return w->entity<>(); })
            .endClass()
            .beginClass<flecs::entity>("entity")
                .addProperty("is_alive", +[](const flecs::entity* e) { return e->is_alive(); })
                .addFunction("destruct", &flecs::entity::destruct)
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
                .addFunction("subscribe", luabridge::overload<
                    const event_type&, unique_id, luabridge::LuaRef
                >(&EventManager::subscribe))
            .endClass()

            .beginClass<VirtualFS>("VirtualFS")
                .addFunction("get_path", &VirtualFS::getPath)
                .addFunction("file_exists", &VirtualFS::fileExists)
                /*.addFunction("loadBinaryData",
                    luabridge::overload<const std::string&, std::vector<char>& >(&VirtualFS::loadBinaryData),
                    luabridge::overload<std::ifstream&, std::vector<char>& >(&VirtualFS::loadBinaryData)
                    )
                .addFunction("loadRGBAImage", &VirtualFS::loadImage)
                .addFunction("destroyImage", &VirtualFS::destroyImage)
                .addFunction("loadScript", &VirtualFS::loadScript)
                .addFunction("loadShader", &VirtualFS::loadShader)*/
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
                .addConstructor<void(Context&, unique_id)>()
                .addStaticProperty("ID", &RenderingWindowManager::ID)
               .addFunction("create", &RenderingWindowManager::create)
               .addFunction("get",
                   luabridge::nonConstOverload<const unique_id>(&RenderingWindowManager::get),
                   luabridge::constOverload<const unique_id>(&RenderingWindowManager::get)
                   )
            .endClass()

            .beginClass<ViewportConfiguration>("ViewportConfiguration")
                .addConstructor<void()>()
                .addProperty("render_target", &ViewportConfiguration::renderTarget)
                .addProperty("viewport", &ViewportConfiguration::viewport)
                .addProperty("scissors", &ViewportConfiguration::scissors)
            .endClass()
            .beginClass<Viewport>("Viewport")
                .addProperty("id", &Viewport::id)
                .addProperty("render_target", &Viewport::getRenderTarget)
            .endClass()
            .beginClass<ViewportManager>("ViewportManager")
                .addConstructor<void(Context&, unique_id)>()
                .addStaticProperty("ID", &ViewportManager::ID)
                .addFunction("create", &ViewportManager::create<ViewportConfiguration>)
                .addFunction("get",
                    luabridge::nonConstOverload<const unique_id>(&ViewportManager::get),
                    luabridge::constOverload<const unique_id>(&ViewportManager::get)
                )
                .addFunction("destroyAll", &ViewportManager::destroy)
                .addFunction("destroy",
                  luabridge::overload<const unique_id>(&Manager<Viewport>::destroy))
            .endClass()

            .beginClass<RenderTargetConfiguration>("RenderTargetConfiguration")
                .addConstructor<void()>()
                .addProperty("rendering_window_handle", &RenderTargetConfiguration::renderingWindowHandle)
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
               .addFunction("pause", &RenderTarget::pause)
               .addFunction("swap_chain", &RenderTarget::getSwapChain)
               .addFunction("rendering_window_handle", &RenderTarget::getRenderingWindowHandle)
            .endClass()
            .beginClass<RenderTargetManager>("RenderTargetManager")
                .addConstructor<void(Context&, unique_id)>()
                .addStaticProperty("ID", &RenderTargetManager::ID)
                .addFunction("create", &ResourcesManager<RenderTarget>::create<RenderTargetConfiguration>)
                .addFunction("get",
                    luabridge::nonConstOverload<const unique_id>(&RenderTargetManager::get),
                    luabridge::constOverload<const unique_id>(&RenderTargetManager::get)
                    )
                .addFunction("destroy",
                   luabridge::overload<const unique_id>(&Manager<RenderTarget>::destroy),
                   luabridge::overload<const void*>(&RenderTargetManager::destroy)
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
                .addProperty("swap_chain_format", &RendererConfiguration::swapChainFormat)
                .addProperty("present_mode", &RendererConfiguration::presentMode)
                .addProperty("frames_in_flight", &RendererConfiguration::framesInFlight)
                .addProperty("color_rendering_format", &RendererConfiguration::colorRenderingFormat)
                .addProperty("depth_stencil_format", &RendererConfiguration::depthStencilFormat)
                .addProperty("clear_color", &RendererConfiguration::clearColor)
                .addProperty("msaa", &RendererConfiguration::msaa)
            .endClass()
            .beginClass<Renderer>("Renderer")
            .endClass()

            .beginClass<ResourcesLocator>("ResourcesLocator")
                .addFunction("get", &ResourcesLocator::_getManager)
                .addProperty("render_target_manager", +[](const ResourcesLocator* rl) -> RenderTargetManager& {
                        return rl->get<RenderTargetManager>(RenderTargetManager::ID);
                    })
                .addProperty("viewport_manager", +[](const ResourcesLocator* rl) -> ViewportManager& {
                        return rl->get<ViewportManager>(ViewportManager::ID);
                    })
                .addProperty("rendering_window_manager", +[](const ResourcesLocator* rl) -> RenderingWindowManager& {
                    return rl->get<RenderingWindowManager>(RenderingWindowManager::ID);
                })
            .endClass()

            .beginClass<Context>("Context")
                .addProperty("exit", &Context::exit)
                .addProperty("vireo", +[](const Context* self) { return self->vireo; })
                .addProperty("virtual_fs",  +[](const Context* self) -> const VirtualFS& { return self->virtualFs; })
                .addProperty("event_manager", +[](const Context* self) -> const EventManager& { return self->eventManager; })
                .addProperty("world", +[](const Context* self) -> const flecs::world& { return self->world; })
                .addProperty("resources_locator", +[](const Context* self) -> const ResourcesLocator& { return self->resourcesLocator; })
                .addProperty("graphic_queue", +[](const Context* self) { return self->graphicQueue; })
            .endClass()
        .endNamespace();
    }
}