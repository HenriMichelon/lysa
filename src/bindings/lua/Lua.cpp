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

import lua_bridge;
import vireo.lua;
import lysa;

template <> struct luabridge::Stack<lysa::LogLevel> : Enum<lysa::LogLevel> {};
template <> struct luabridge::Stack<lysa::LoggingMode> : Enum<lysa::LoggingMode> {};
template <> struct luabridge::Stack<lysa::RendererType> : Enum<lysa::RendererType> {};
template <> struct luabridge::Stack<lysa::RenderingWindowMode> : Enum<lysa::RenderingWindowMode> {};
template <> struct luabridge::Stack<lysa::InputEventType> : Enum<lysa::InputEventType> {};
template <> struct luabridge::Stack<lysa::KeyModifier> : Enum<lysa::KeyModifier> {};
template <> struct luabridge::Stack<lysa::Key> : Enum<lysa::Key> {};
template <> struct luabridge::Stack<lysa::MouseButton> : Enum<lysa::MouseButton> {};
template <> struct luabridge::Stack<lysa::GamepadButton> : Enum<lysa::GamepadButton> {};
template <> struct luabridge::Stack<lysa::GamepadAxisJoystick> : Enum<lysa::GamepadAxisJoystick> {};
template <> struct luabridge::Stack<lysa::GamepadAxis> : Enum<lysa::GamepadAxis> {};
template <> struct luabridge::Stack<lysa::MouseMode> : Enum<lysa::MouseMode> {};
template <> struct luabridge::Stack<lysa::MouseCursor> : Enum<lysa::MouseCursor> {};

namespace lysa {

    Lua::Lua(Context& ctx, const LuaConfiguration& luaConfiguration) :
        ctx(ctx),
        L(luaL_newstate()) {
        luaL_openlibs(L);
        luaL_requiref(L, "socket", luaopen_socket_core, 1);
        lua_pop(L, 1);
        luaL_requiref(L, "socket.core", luaopen_socket_core, 1);
        lua_pop(L, 1);
        luabridge::enableExceptions(L);
        bind();

        const std::string path_chunk = "package.path = package.path .. ';?.lua;" + ctx.fs.getScriptsDirectory()  + "/?.lua'";
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

    void Lua::pushSandboxEnv() const {
        // env = {}
        lua_newtable(L);
        // env._G = env
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "_G");
        // mt = {}
        lua_newtable(L);
        // mt.__index = _G
        lua_pushglobaltable(L);
        lua_setfield(L, -2, "__index");
        // setmetatable(env, mt)
        lua_setmetatable(L, -2);
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
           .addConstructor<void(float), void(float, float)>()
           .addProperty("x", &float2::x)
           .addProperty("r", &float2::r)
           .addProperty("y", &float2::y)
           .addProperty("g", &float2::g)
       .endClass()
       .beginClass<float3>("float3")
           .addConstructor<void(float), void(float, float, float)>()
           .addProperty("x", &float3::x)
           .addProperty("r", &float3::r)
           .addProperty("y", &float3::y)
           .addProperty("g", &float3::g)
           .addProperty("z", &float3::z)
           .addProperty("b", &float3::b)
       .endClass()
       .beginClass<float4>("float4")
           .addConstructor<void(float), void(float, float, float, float)>()
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
        
        .beginNamespace("Key")

            .addProperty("KEY_NONE", +[]{ return (uint32)KEY_NONE; })
            .addProperty("KEY_SPACE", +[]{ return (uint32)KEY_SPACE; })
            .addProperty("KEY_DASH", +[]{ return (uint32)KEY_DASH; })
            .addProperty("KEY_PIPE", +[]{ return (uint32)KEY_PIPE; })
            .addProperty("KEY_APOSTROPHE", +[]{ return (uint32)KEY_APOSTROPHE; })
            .addProperty("KEY_COMMA", +[]{ return (uint32)KEY_COMMA; })
            .addProperty("KEY_PERIOD", +[]{ return (uint32)KEY_PERIOD; })
            .addProperty("KEY_QUESTIONMARK", +[]{ return (uint32)KEY_QUESTIONMARK; })

            .addProperty("KEY_0", +[]{ return (uint32)KEY_0; })
            .addProperty("KEY_1", +[]{ return (uint32)KEY_1; })
            .addProperty("KEY_2", +[]{ return (uint32)KEY_2; })
            .addProperty("KEY_3", +[]{ return (uint32)KEY_3; })
            .addProperty("KEY_4", +[]{ return (uint32)KEY_4; })
            .addProperty("KEY_5", +[]{ return (uint32)KEY_5; })
            .addProperty("KEY_6", +[]{ return (uint32)KEY_6; })
            .addProperty("KEY_7", +[]{ return (uint32)KEY_7; })
            .addProperty("KEY_8", +[]{ return (uint32)KEY_8; })
            .addProperty("KEY_9", +[]{ return (uint32)KEY_9; })

            .addProperty("KEY_SEMICOLON", +[]{ return (uint32)KEY_SEMICOLON; })
            .addProperty("KEY_EQUAL", +[]{ return (uint32)KEY_EQUAL; })

            .addProperty("KEY_A", +[]{ return (uint32)KEY_A; })
            .addProperty("KEY_B", +[]{ return (uint32)KEY_B; })
            .addProperty("KEY_C", +[]{ return (uint32)KEY_C; })
            .addProperty("KEY_D", +[]{ return (uint32)KEY_D; })
            .addProperty("KEY_E", +[]{ return (uint32)KEY_E; })
            .addProperty("KEY_F", +[]{ return (uint32)KEY_F; })
            .addProperty("KEY_G", +[]{ return (uint32)KEY_G; })
            .addProperty("KEY_H", +[]{ return (uint32)KEY_H; })
            .addProperty("KEY_I", +[]{ return (uint32)KEY_I; })
            .addProperty("KEY_J", +[]{ return (uint32)KEY_J; })
            .addProperty("KEY_K", +[]{ return (uint32)KEY_K; })
            .addProperty("KEY_L", +[]{ return (uint32)KEY_L; })
            .addProperty("KEY_M", +[]{ return (uint32)KEY_M; })
            .addProperty("KEY_N", +[]{ return (uint32)KEY_N; })
            .addProperty("KEY_O", +[]{ return (uint32)KEY_O; })
            .addProperty("KEY_P", +[]{ return (uint32)KEY_P; })
            .addProperty("KEY_Q", +[]{ return (uint32)KEY_Q; })
            .addProperty("KEY_R", +[]{ return (uint32)KEY_R; })
            .addProperty("KEY_S", +[]{ return (uint32)KEY_S; })
            .addProperty("KEY_T", +[]{ return (uint32)KEY_T; })
            .addProperty("KEY_U", +[]{ return (uint32)KEY_U; })
            .addProperty("KEY_V", +[]{ return (uint32)KEY_V; })
            .addProperty("KEY_W", +[]{ return (uint32)KEY_W; })
            .addProperty("KEY_X", +[]{ return (uint32)KEY_X; })
            .addProperty("KEY_Y", +[]{ return (uint32)KEY_Y; })
            .addProperty("KEY_Z", +[]{ return (uint32)KEY_Z; })

            .addProperty("KEY_LEFT_BRACKET", +[]{ return (uint32)KEY_LEFT_BRACKET; })
            .addProperty("KEY_BACKSLASH", +[]{ return (uint32)KEY_BACKSLASH; })
            .addProperty("KEY_RIGHT_BRACKET", +[]{ return (uint32)KEY_RIGHT_BRACKET; })
            .addProperty("KEY_GRAVE_ACCENT", +[]{ return (uint32)KEY_GRAVE_ACCENT; })

            .addProperty("KEY_ESCAPE", +[]{ return (uint32)KEY_ESCAPE; })
            .addProperty("KEY_ENTER", +[]{ return (uint32)KEY_ENTER; })
            .addProperty("KEY_TAB", +[]{ return (uint32)KEY_TAB; })
            .addProperty("KEY_BACKSPACE", +[]{ return (uint32)KEY_BACKSPACE; })

            .addProperty("KEY_INSERT", +[]{ return (uint32)KEY_INSERT; })
            .addProperty("KEY_DELETE", +[]{ return (uint32)KEY_DELETE; })

            .addProperty("KEY_RIGHT", +[]{ return (uint32)KEY_RIGHT; })
            .addProperty("KEY_LEFT", +[]{ return (uint32)KEY_LEFT; })
            .addProperty("KEY_DOWN", +[]{ return (uint32)KEY_DOWN; })
            .addProperty("KEY_UP", +[]{ return (uint32)KEY_UP; })

            .addProperty("KEY_PAGE_UP", +[]{ return (uint32)KEY_PAGE_UP; })
            .addProperty("KEY_PAGE_DOWN", +[]{ return (uint32)KEY_PAGE_DOWN; })
            .addProperty("KEY_HOME", +[]{ return (uint32)KEY_HOME; })
            .addProperty("KEY_END", +[]{ return (uint32)KEY_END; })

            .addProperty("KEY_CAPS_LOCK", +[]{ return (uint32)KEY_CAPS_LOCK; })
            .addProperty("KEY_SCROLL_LOCK", +[]{ return (uint32)KEY_SCROLL_LOCK; })
            .addProperty("KEY_NUM_LOCK", +[]{ return (uint32)KEY_NUM_LOCK; })
            .addProperty("KEY_PRINT_SCREEN", +[]{ return (uint32)KEY_PRINT_SCREEN; })
            .addProperty("KEY_PAUSE", +[]{ return (uint32)KEY_PAUSE; })

            .addProperty("KEY_F1", +[]{ return (uint32)KEY_F1; })
            .addProperty("KEY_F2", +[]{ return (uint32)KEY_F2; })
            .addProperty("KEY_F3", +[]{ return (uint32)KEY_F3; })
            .addProperty("KEY_F4", +[]{ return (uint32)KEY_F4; })
            .addProperty("KEY_F5", +[]{ return (uint32)KEY_F5; })
            .addProperty("KEY_F6", +[]{ return (uint32)KEY_F6; })
            .addProperty("KEY_F7", +[]{ return (uint32)KEY_F7; })
            .addProperty("KEY_F8", +[]{ return (uint32)KEY_F8; })
            .addProperty("KEY_F9", +[]{ return (uint32)KEY_F9; })
            .addProperty("KEY_F10", +[]{ return (uint32)KEY_F10; })
            .addProperty("KEY_F11", +[]{ return (uint32)KEY_F11; })
            .addProperty("KEY_F12", +[]{ return (uint32)KEY_F12; })

            .addProperty("KEY_KP_0", +[]{ return (uint32)KEY_KP_0; })
            .addProperty("KEY_KP_1", +[]{ return (uint32)KEY_KP_1; })
            .addProperty("KEY_KP_2", +[]{ return (uint32)KEY_KP_2; })
            .addProperty("KEY_KP_3", +[]{ return (uint32)KEY_KP_3; })
            .addProperty("KEY_KP_4", +[]{ return (uint32)KEY_KP_4; })
            .addProperty("KEY_KP_5", +[]{ return (uint32)KEY_KP_5; })
            .addProperty("KEY_KP_6", +[]{ return (uint32)KEY_KP_6; })
            .addProperty("KEY_KP_7", +[]{ return (uint32)KEY_KP_7; })
            .addProperty("KEY_KP_8", +[]{ return (uint32)KEY_KP_8; })
            .addProperty("KEY_KP_9", +[]{ return (uint32)KEY_KP_9; })

            .addProperty("KEY_KP_PERIOD", +[]{ return (uint32)KEY_KP_PERIOD; })
            .addProperty("KEY_KP_DIVIDE", +[]{ return (uint32)KEY_KP_DIVIDE; })
            .addProperty("KEY_KP_MULTIPLY", +[]{ return (uint32)KEY_KP_MULTIPLY; })
            .addProperty("KEY_KP_SUBTRACT", +[]{ return (uint32)KEY_KP_SUBTRACT; })
            .addProperty("KEY_KP_ADD", +[]{ return (uint32)KEY_KP_ADD; })
            .addProperty("KEY_KP_ENTER", +[]{ return (uint32)KEY_KP_ENTER; })
            .addProperty("KEY_KP_EQUAL", +[]{ return (uint32)KEY_KP_EQUAL; })

            .addProperty("KEY_LEFT_SHIFT", +[]{ return (uint32)KEY_LEFT_SHIFT; })
            .addProperty("KEY_LEFT_CONTROL", +[]{ return (uint32)KEY_LEFT_CONTROL; })
            .addProperty("KEY_LEFT_ALT", +[]{ return (uint32)KEY_LEFT_ALT; })
            .addProperty("KEY_LEFT_SUPER", +[]{ return (uint32)KEY_LEFT_SUPER; })

            .addProperty("KEY_RIGHT_SHIFT", +[]{ return (uint32)KEY_RIGHT_SHIFT; })
            .addProperty("KEY_RIGHT_CONTROL", +[]{ return (uint32)KEY_RIGHT_CONTROL; })
            .addProperty("KEY_RIGHT_ALT", +[]{ return (uint32)KEY_RIGHT_ALT; })
            .addProperty("KEY_RIGHT_SUPER", +[]{ return (uint32)KEY_RIGHT_SUPER; })

        .endNamespace()


        .beginNamespace("InputEventType")
            .addVariable("KEY", InputEventType::KEY)
            .addVariable("MOUSE_MOTION", InputEventType::MOUSE_MOTION)
            .addVariable("MOUSE_BUTTON", InputEventType::MOUSE_BUTTON)
            .addVariable("GAMEPAD_BUTTON", InputEventType::GAMEPAD_BUTTON)
        .endNamespace()
        .beginClass<InputEventKey>("InputEventKey")
            .addProperty("key", &InputEventKey::key)
            .addProperty("pressed", &InputEventKey::pressed)
            .addProperty("repeat", &InputEventKey::repeat)
            .addProperty("modifiers", &InputEventKey::modifiers)
        .endClass()
        .beginClass<InputEvent>("InputEvent")
            .addProperty("type", &InputEvent::type)
            .addProperty("input_event_key", +[](const InputEvent*e) { return std::get<InputEventKey>(e->data);})
        .endClass()

        .beginClass<Event>("Event")
            .addProperty("id", &Event::id)
            .addProperty("type", &Event::type)
            .addFunction("get_float", +[](const Event*e) { return std::any_cast<float>(e->payload);})
            .addFunction("get_int32", +[](const Event*e) { return std::any_cast<uint32>(e->payload);})
            .addFunction("get_input_event", +[](const Event*e) { return std::any_cast<const InputEvent>(e->payload);})
        .endClass()
        .beginClass<EventManager>("EventManager")
            .addFunction("h", +[](EventManager*){ Log::info("h"); })
            .addFunction("push", &EventManager::push)
            .addFunction("fire", &EventManager::fire)
            .addFunction("subscribe",
                luabridge::overload<const event_type&, unique_id, const luabridge::LuaRef&>(&EventManager::subscribe),
                luabridge::overload<const event_type&, const luabridge::LuaRef&>(&EventManager::subscribe)
            )
            .addFunction("unsubscribe",
                luabridge::overload<const event_type&, unique_id, const luabridge::LuaRef&>(&EventManager::unsubscribe),
                luabridge::overload<const event_type&, const luabridge::LuaRef&>(&EventManager::unsubscribe)
            )
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
            .addVariable("INPUT", &RenderingWindowEvent::INPUT)
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
               luabridge::nonConstOverload<const unique_id>(&RenderingWindowManager::operator[]),
               luabridge::constOverload<const unique_id>(&RenderingWindowManager::operator[])
               )
            .addFunction("destroy", &RenderingWindowManager::destroy)
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
                luabridge::nonConstOverload<const unique_id>(&RenderTargetManager::operator[]),
                luabridge::constOverload<const unique_id>(&RenderTargetManager::operator[])
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
               luabridge::nonConstOverload<const unique_id>(&ImageManager::operator[]),
               luabridge::constOverload<const unique_id>(&ImageManager::operator[])
               )
            .addFunction("destroy",&ImageManager::destroy)
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
              luabridge::nonConstOverload<const unique_id>(&ImageTextureManager::operator[]),
              luabridge::constOverload<const unique_id>(&ImageTextureManager::operator[])
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
            .addProperty("type", &Material::getType)
            // .addProperty("cull_mode", &Material::getCullMode, &Material::setCullMode)
            // .addProperty("transparency", &Material::getTransparency, &Material::setTransparency)
            // .addProperty("alpha_scissor", &Material::getAlphaScissor, &Material::setAlphaScissor)
            // .addProperty("index", &Material::getIndex)
        .endClass()
        .beginClass<StandardMaterial::TextureInfo>("TextureInfo")
            .addConstructor<void(const ImageTexture*)>()
            .addProperty("texture", &StandardMaterial::TextureInfo::texture)
            .addProperty("transform", &StandardMaterial::TextureInfo::transform)
        .endClass()
        .beginClass<StandardMaterial>("StandardMaterial")
            .addProperty("id", &StandardMaterial::id)
            // .addProperty("type", &StandardMaterial::getType)
            .addProperty("cull_mode",
                +[](const StandardMaterial* self) -> vireo::CullMode {
                    return self->getCullMode();
                },
                +[](StandardMaterial* self, vireo::CullMode cullmode) {
                    return self->setCullMode(cullmode);
                }
            )
            .addProperty("transparency",
                +[](const StandardMaterial* self) -> uint32 {
                    return static_cast<uint32>(self->getTransparency());
                },
                +[](StandardMaterial* self, uint32 t) {
                    return self->setTransparency(static_cast<Transparency>(t));
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
            // .addProperty("type", &ShaderMaterial::getType)
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
                 luabridge::nonConstOverload<const unique_id>(&MaterialManager::operator[]),
                 luabridge::constOverload<const unique_id>(&MaterialManager::operator[])
                 )
            .addFunction("destroy",&MaterialManager::destroy)
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
             luabridge::nonConstOverload<const unique_id>(&MeshManager::operator[]),
             luabridge::constOverload<const unique_id>(&MeshManager::operator[])
             )
            .addFunction("destroy",&MeshManager::destroy)
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
             luabridge::nonConstOverload<const unique_id>(&SceneManager::operator[]),
             luabridge::constOverload<const unique_id>(&SceneManager::operator[])
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

        .beginNamespace("MainLoopEvent")
            .addVariable("PROCESS", MainLoopEvent::PROCESS)
            .addVariable("PHYSICS_PROCESS", MainLoopEvent::PHYSICS_PROCESS)
            .addVariable("QUIT", MainLoopEvent::QUIT)
        .endNamespace()

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

        .beginClass<Context>("Context")
           .addProperty("exit", [this](Context*) { return &ctx.exit;})
           .addProperty("vireo", [this](Context*) { return ctx.vireo;})
           .addProperty("fs",  [this](Context*) { return &ctx.fs;})
           .addProperty("events", [this](Context*) { return &ctx.events;})
   #ifdef ECS_SCENES
           .addProperty("world", [this](Context*) { return &ctx.world;})
   #endif
           .addProperty("res", [this](Context*) { return &ctx.res;})
           .addProperty("graphic_queue", [this](Context*) { return ctx.graphicQueue;})
       .endClass()

        .endNamespace();


#ifdef ECS_SCENES
        beginNamespace("ecs")
            .beginClass<ecs::RenderTarget>("RenderTarget")
                .addConstructor<void(unique_id)>()
                .addProperty("render_target", &ecs::RenderTarget::renderTarget)
            .endClass()
            .beginClass<ecs::Viewport>("Viewport")
                .addConstructor<void(vireo::Viewport), void(vireo::Viewport, vireo::Rect)>()
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
                .addConstructor<void(flecs::entity)>()
                .addProperty("camera", &ecs::CameraRef::camera)
            .endClass()
            .beginClass<ecs::MaterialOverride>("MaterialOverride")
                .addConstructor<void(), void(uint32, unique_id)>()
                .addProperty("surface_index", &ecs::MaterialOverride::surfaceIndex)
                .addProperty("material", &ecs::MaterialOverride::material)
            .endClass()
            .beginClass<ecs::MeshInstance>("MeshInstance")
                .addConstructor<void(unique_id)>()
                .addProperty("mesh", &ecs::MeshInstance::mesh)
            .endClass()
            .beginClass<ecs::Scene>("Scene")
                .addConstructor<void(unique_id)>()
                .addProperty("scene", &ecs::Scene::scene)
            .endClass()
            .beginClass<ecs::SceneRef>("SceneRef")
                .addConstructor<void(flecs::entity)>()
                .addProperty("scene", &ecs::SceneRef::scene)
            .endClass()
            .beginClass<ecs::AmbientLight>("AmbientLight")
                .addConstructor<void()>()
                .addProperty("color", &ecs::AmbientLight::color)
                .addProperty("intensity", &ecs::AmbientLight::intensity)
            .endClass()
            .beginClass<ecs::Visible>("Visible")
                .addConstructor<void()>()
            .endClass()
            .beginClass<ecs::Transform>("Transform")
                .addConstructor<void()>()
                .addProperty("local", &ecs::Transform::local)
                .addProperty("global", &ecs::Transform::global)
            .endClass()

            .addFunction("set_position",
                luabridge::overload<flecs::entity, const float3&>(&ecs::setPosition),
                luabridge::overload<flecs::entity, float, float, float>(&ecs::setPosition)
            )
            .addFunction("translate",
                luabridge::overload<flecs::entity, const float3&>(&ecs::translate),
                luabridge::overload<flecs::entity, float, float, float>(&ecs::translate)
            )
            .addFunction("rotate_x",&ecs::rotateX)
            .addFunction("rotate_y",&ecs::rotateY)
            .addFunction("rotate_z",&ecs::rotateZ)

            .beginClass<flecs::world>("world")
                .addFunction("entity", +[](const flecs::world* w) { return w->entity<>(); })
            .endClass()
            .addProperty("child_of", +[]{ return flecs::ChildOf;})

            .beginClass<flecs::entity>("entity")
                .addProperty("is_alive", +[](const flecs::entity* e) { return e->is_alive(); })
                .addFunction("destruct", &flecs::entity::destruct)
                .addFunction("child_of", +[](const flecs::entity* e, const flecs::entity p) {
                    return e->add(flecs::ChildOf, p);
                })
                .addFunction("add",
                    luabridge::overload<const flecs::entity*, const flecs::entity>(+[](const flecs::entity* e, const flecs::entity c) {
                       return e->set(c);
                    }),
                    luabridge::overload<const flecs::entity*, const flecs::entity, const flecs::entity>(+[](const flecs::entity* e, const flecs::entity f, const flecs::entity s) {
                      return e->add(f, s);
                    }),
                    luabridge::overload<const flecs::entity*, const flecs::entity_t, const flecs::entity>(+[](const flecs::entity* e, const flecs::entity_t f, const flecs::entity s) {
                      return e->add(f, s);
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::RenderTarget&>(+[](const flecs::entity* e, const ecs::RenderTarget& p) {
                        return e->set<ecs::RenderTarget>(p);
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::Viewport&>(+[](const flecs::entity* e, const ecs::Viewport& p) {
                        return e->set<ecs::Viewport>(p);
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::Camera&>(+[](const flecs::entity* e, const ecs::Camera& p) {
                        return e->set<ecs::Camera>(p);
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::CameraRef&>(+[](const flecs::entity* e, const ecs::CameraRef& p) {
                        return e->set<ecs::CameraRef>(p);
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::MaterialOverride&>(+[](const flecs::entity* e, const ecs::MaterialOverride& p) {
                        return e->set<ecs::MaterialOverride>(p);
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::MeshInstance&>(+[](const flecs::entity* e, const ecs::MeshInstance& p) {
                        return e->set<ecs::MeshInstance>(p);
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::Scene&>(+[](const flecs::entity* e, const ecs::Scene& p) {
                        return e->set<ecs::Scene>(p);
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::SceneRef&>(+[](const flecs::entity* e, const ecs::SceneRef& p) {
                        return e->set<ecs::SceneRef>(p);
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::AmbientLight&>(+[](const flecs::entity* e, const ecs::AmbientLight& p) {
                        return e->set<ecs::AmbientLight>(p);
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::Visible&>(+[](const flecs::entity* e, const ecs::Visible&) {
                        return e->add<ecs::Visible>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::Transform&>(+[](const flecs::entity* e, const ecs::Transform& p) {
                        return e->set<ecs::Transform>(p);
                    })
                    )
                .addFunction("has",
                    luabridge::overload<const flecs::entity*, const flecs::entity>(+[](const flecs::entity* e, const flecs::entity c) {
                       return e->has(c);
                   }),
                   luabridge::overload<const flecs::entity*, const ecs::RenderTarget&>(+[](const flecs::entity* e, const ecs::RenderTarget& p) {
                        return e->has<ecs::RenderTarget>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::Viewport&>(+[](const flecs::entity* e, const ecs::Viewport& p) {
                        return e->has<ecs::Viewport>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::Camera&>(+[](const flecs::entity* e, const ecs::Camera& p) {
                        return e->has<ecs::Camera>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::CameraRef&>(+[](const flecs::entity* e, const ecs::CameraRef& p) {
                        return e->has<ecs::CameraRef>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::MaterialOverride&>(+[](const flecs::entity* e, const ecs::MaterialOverride& p) {
                        return e->has<ecs::MaterialOverride>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::MeshInstance&>(+[](const flecs::entity* e, const ecs::MeshInstance& p) {
                        return e->has<ecs::MeshInstance>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::Scene&>(+[](const flecs::entity* e, const ecs::Scene& p) {
                        return e->has<ecs::Scene>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::SceneRef&>(+[](const flecs::entity* e, const ecs::SceneRef& p) {
                        return e->has<ecs::SceneRef>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::AmbientLight&>(+[](const flecs::entity* e, const ecs::AmbientLight& p) {
                        return e->has<ecs::AmbientLight>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::Visible&>(+[](const flecs::entity* e, const ecs::Visible&) {
                        return e->has<ecs::Visible>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::Transform&>(+[](const flecs::entity* e, const ecs::Transform& p) {
                        return e->has<ecs::Transform>();
                    })
                )
                .addFunction("remove",
                     luabridge::overload<const flecs::entity*, const flecs::entity>(+[](const flecs::entity* e, const flecs::entity c) {
                        return e->remove(c);
                    }),
                    luabridge::overload<const flecs::entity*, const flecs::entity, const flecs::entity>(+[](const flecs::entity* e, const flecs::entity f, const flecs::entity s) {
                       return e->remove(f, s);
                    }),
                    luabridge::overload<const flecs::entity*, const flecs::entity_t, const flecs::entity>(+[](const flecs::entity* e, const flecs::entity_t f, const flecs::entity s) {
                       return e->remove(f, s);
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::RenderTarget&>(+[](const flecs::entity* e, const ecs::RenderTarget& p) {
                        return e->remove<ecs::RenderTarget>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::Viewport&>(+[](const flecs::entity* e, const ecs::Viewport& p) {
                        return e->remove<ecs::Viewport>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::Camera&>(+[](const flecs::entity* e, const ecs::Camera& p) {
                        return e->remove<ecs::Camera>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::CameraRef&>(+[](const flecs::entity* e, const ecs::CameraRef& p) {
                        return e->remove<ecs::CameraRef>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::MaterialOverride&>(+[](const flecs::entity* e, const ecs::MaterialOverride& p) {
                        return e->remove<ecs::MaterialOverride>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::MeshInstance&>(+[](const flecs::entity* e, const ecs::MeshInstance& p) {
                        return e->remove<ecs::MeshInstance>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::Scene&>(+[](const flecs::entity* e, const ecs::Scene& p) {
                        return e->remove<ecs::Scene>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::SceneRef&>(+[](const flecs::entity* e, const ecs::SceneRef& p) {
                        return e->remove<ecs::SceneRef>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::AmbientLight&>(+[](const flecs::entity* e, const ecs::AmbientLight& p) {
                        return e->remove<ecs::AmbientLight>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::Visible&>(+[](const flecs::entity* e, const ecs::Visible&) {
                        return e->remove<ecs::Visible>();
                    }),
                    luabridge::overload<const flecs::entity*, const ecs::Transform&>(+[](const flecs::entity* e, const ecs::Transform& p) {
                        return e->remove<ecs::Transform>();
                    })
                )
                .addProperty("render_target", [](const flecs::entity* e) { return e->get<ecs::RenderTarget>(); })
                .addProperty("viewport", [](const flecs::entity* e) { return e->get<ecs::Viewport>(); })
                .addProperty("camera", [](const flecs::entity* e) { return e->get_mut<ecs::Camera>(); })
                .addProperty("camera_ref", [](const flecs::entity* e) { return e->get<ecs::CameraRef>(); })
                .addProperty("material_override", [](const flecs::entity* e) { return e->get<ecs::MaterialOverride>(); })
                .addProperty("mesh_instance", [](const flecs::entity* e) { return e->get<ecs::MeshInstance>(); })
                .addProperty("scene_ref", [](const flecs::entity* e) { return e->get<ecs::SceneRef>(); })
                .addProperty("ambient_light", [](const flecs::entity* e) { return e->get<ecs::AmbientLight>(); })
                .addProperty("transform", [](const flecs::entity* e) { return e->get<ecs::Transform>(); })
            .endClass()
        .endNamespace();
#endif
    }
}