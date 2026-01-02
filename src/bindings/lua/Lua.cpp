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

        // clear loaded modules
        lua_getglobal(L, "package");
        lua_getfield(L, -1, "loaded");
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
            lua_pop(L, 1);
            // key (package name)
            lua_pushvalue(L, -1);
            lua_pushnil(L);
            // loaded[key] = nil
            lua_settable(L, -4);
        }
        // loaded, package
        lua_pop(L, 2);
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
            .addConstructor<void(), void(float), void(float, float)>()
            .addProperty("is_zero", [&](const float2* f) { return all(*f == FLOAT2ZERO); })
            .addProperty("x",
               [&](const float2* f) -> float {return f->f32[0];},
               [&](float2* f, const float v) { f->f32[0] = v;}
            )
            .addProperty("r",
                [&](const float2* f) -> float {return f->f32[0];},
                [&](float2* f, const float v) { f->f32[0] = v;}
             )
            .addProperty("y",
                [&](const float2* f) -> float {return f->f32[1];},
                [&](float2* f, const float v) { f->f32[1] = v;}
             )
             .addProperty("g",
                 [&](const float2* f) -> float {return f->f32[1];},
                 [&](float2* f, const float v) { f->f32[1] = v;}
              )
        .endClass()
       .beginClass<float3>("float3")
            .addConstructor<void(), void(float), void(float, float, float)>()
            .addProperty("is_zero", [&](const float3* f) { return all(*f == FLOAT3ZERO); })
            .addProperty("x",
                [&](const float3* f) -> float {return f->f32[0];},
                [&](float3* f, const float v) { f->f32[0] = v;}
             )
            .addProperty("r",
                [&](const float3* f) -> float {return f->f32[0];},
                [&](float3* f, const float v) { f->f32[0] = v;}
            )
            .addProperty("y",
                [&](const float3* f) -> float {return f->f32[1];},
                [&](float3* f, const float v) { f->f32[1] = v;}
            )
            .addProperty("g",
                [&](const float3* f) -> float {return f->f32[1];},
                [&](float3* f, const float v) { f->f32[1] = v;}
             )
            .addProperty("z",
                [&](const float3* f) -> float {return f->f32[2];},
                [&](float3* f, const float v) { f->f32[2] = v;}
             )
            .addProperty("b",
                 [&](const float3* f) -> float {return f->f32[2];},
                 [&](float3* f, const float v) { f->f32[2] = v;}
            )
        .endClass()
        .beginClass<float1x2>("float1x2")
        .endClass()
        .beginClass<float2x2>("float2x2")
        .endClass()
        .beginClass<float3x2>("float3x2")
        .endClass()
        .beginClass<float4x2>("float4x2")
        .endClass()
        .beginClass<float1x3>("float1x3")
        .endClass()
        .beginClass<float2x3>("float2x3")
        .endClass()
        .beginClass<float3x3>("float3x3")
        .endClass()
        .beginClass<float4x3>("float4x3")
        .endClass()
        .beginClass<float1x4>("float1x4")
        .endClass()
        .beginClass<float2x4>("float2x4")
        .endClass()
        .beginClass<float3x4>("float3x4")
        .endClass()
        .beginClass<float4x4>("float4x4")
            .addStaticFunction("identity", &float4x4::identity)
            .addStaticFunction("translation",
                luabridge::overload<float, float, float>(&float4x4::translation),
                luabridge::overload<const float3&>(&float4x4::translation)
            )
            .addStaticFunction("scale",
                luabridge::overload<float, float, float>(&float4x4::scale),
                luabridge::overload<const float3&>(&float4x4::scale),
                luabridge::overload<float>(&float4x4::scale)
            )
                .addStaticFunction("rotation_x", &float4x4::rotation_x)
            .addStaticFunction("rotation_y", &float4x4::rotation_y)
            .addStaticFunction("rotation_z", &float4x4::rotation_z)
        .endClass()
        .beginClass<float4>("float4")
            .addConstructor<void(), void(float), void(float, float, float, float), void(float3, float)>()
            .addProperty("is_zero", [&](const float4* f) { return all(*f == FLOAT4ZERO); })
            .addProperty("x",
                [&](const float4* f) -> float {return f->f32[0];},
                [&](float4* f, const float v) { f->f32[0] = v;}
             )
            .addProperty("r",
                [&](const float4* f) -> float {return f->f32[0];},
                [&](float4* f, const float v) { f->f32[0] = v;}
            )
            .addProperty("y",
                [&](const float4* f) -> float {return f->f32[1];},
                [&](float4* f, const float v) { f->f32[1] = v;}
            )
            .addProperty("g",
                [&](const float4* f) -> float {return f->f32[1];},
                [&](float4* f, const float v) { f->f32[1] = v;}
             )
            .addProperty("z",
                [&](const float4* f) -> float {return f->f32[2];},
                [&](float4* f, const float v) { f->f32[2] = v;}
             )
            .addProperty("b",
                [&](const float4* f) -> float {return f->f32[2];},
                [&](float4* f, const float v) { f->f32[2] = v;}
            )
            .addProperty("w",
                [&](const float4* f) -> float {return f->f32[3];},
                [&](float4* f, const float v) { f->f32[3] = v;}
            )
            .addProperty("a",
                [&](const float4* f) -> float {return f->f32[3];},
                [&](float4* f, const float v) { f->f32[3] = v;}
            )
       .endClass()
       .beginClass<quaternion>("quaternion")
           .addConstructor<void(), void(float, float, float, float)>()
            .addProperty("x",
                [&](const quaternion* f) -> float {return f->f32[0];},
                [&](quaternion* f, const float v) { f->f32[0] = v;}
             )
            .addProperty("y",
                [&](const quaternion* f) -> float {return f->f32[1];},
                [&](quaternion* f, const float v) { f->f32[1] = v;}
            )
            .addProperty("z",
                [&](const quaternion* f) -> float {return f->f32[2];},
                [&](quaternion* f, const float v) { f->f32[2] = v;}
             )
            .addProperty("w",
                [&](const quaternion* f) -> float {return f->f32[3];},
                [&](quaternion* f, const float v) { f->f32[3] = v;}
            )
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
        .addFunction("mul",
            luabridge::overload<const float2&, const float>(mul),
            luabridge::overload<const float3&, const float>(mul),
            luabridge::overload<const float4&, const float>(mul),
            luabridge::overload<const float, const float2&>(mul),
            luabridge::overload<const float, const float3&>(mul),
            luabridge::overload<const float, const float4&>(mul),
            luabridge::overload<const float1x2&, const float2&>(mul),
            luabridge::overload<const float2x2&, const float2&>(mul),
            luabridge::overload<const float3x2&, const float2&>(mul),
            luabridge::overload<const float4x2&, const float2&>(mul),
            luabridge::overload<const float1x3&, const float3&>(mul),
            luabridge::overload<const float2x3&, const float3&>(mul),
            luabridge::overload<const float3x3&, const float3&>(mul),
            luabridge::overload<const float3x3&, const float3x1&>(mul),
            luabridge::overload<const float3x3&, const float3x2&>(mul),
            luabridge::overload<const float3x3&, const float3x3&>(mul),
            luabridge::overload<const float4x3&, const float3&>(mul),
            luabridge::overload<const float1x4&, const float4&>(mul),
            luabridge::overload<const float2x4&, const float4&>(mul),
            luabridge::overload<const float3x4&, const float4&>(mul),
            luabridge::overload<const float4x4&, const float4&>(mul),
            luabridge::overload<const float4x4&, const float4x1&>(mul),
            luabridge::overload<const float4x4&, const float4x2&>(mul),
            luabridge::overload<const float4x4&, const float4x3&>(mul),
            luabridge::overload<const float4x4&, const float4x4&>(mul)
        )
        .addFunction("add",
            luabridge::overload<const float2&, const float2&>(add),
            luabridge::overload<const float3&, const float3&>(add),
            luabridge::overload<const float4&, const float4&>(add)
        )
        .addFunction("is_windows", +[]{ return is_windows(); })
        .addFunction("get_current_time_milliseconds", get_current_time_milliseconds)
        .addFunction("sanitize_name", sanitize_name)
        .addFunction("dir_exists", dir_exists)
        .addFunction("to_float3", to_float3)
        .addFunction("to_float4", to_float4)
        .addFunction("to_lower", to_lower)

        .addVariable("AXIS_X", AXIS_X)
        .addVariable("AXIS_Y", AXIS_Y)
        .addVariable("AXIS_Z", AXIS_Z)
        .addVariable("AXIS_UP", AXIS_UP)
        .addVariable("AXIS_DOWN", AXIS_DOWN)
        .addVariable("AXIS_FRONT", AXIS_FRONT)
        .addVariable("AXIS_BACK", AXIS_BACK)
        .addVariable("AXIS_RIGHT", AXIS_RIGHT)
        .addVariable("FLOAT2ZERO", FLOAT2ZERO)
        .addVariable("FLOAT3ZERO", FLOAT3ZERO)
        .addVariable("FLOAT4ZERO", FLOAT4ZERO)
        .addVariable("QUATERNION_IDENTITY", QUATERNION_IDENTITY)
        .addVariable("TRANSFORM_BASIS", TRANSFORM_BASIS)
        .addVariable("HALF_PI", HALF_PI)

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
        .beginClass<InputEventMouseMotion>("InputEventMouseMotion")
            .addProperty("position", &InputEventMouseMotion::position)
            .addProperty("buttonsState", &InputEventMouseMotion::buttonsState)
            .addProperty("modifiers", &InputEventMouseMotion::modifiers)
            .addProperty("relative", &InputEventMouseMotion::relative)
        .endClass()
        .beginNamespace("MouseButton")
            .addVariable("LEFT", MouseButton::LEFT)
            .addVariable("RIGHT", MouseButton::RIGHT)
            .addVariable("MIDDLE", MouseButton::MIDDLE)
            .addVariable("WHEEL", MouseButton::WHEEL)
        .endNamespace()
        .beginClass<InputEventMouseButton>("InputEventMouseButton")
            .addProperty("position", &InputEventMouseButton::position)
            .addProperty("buttonsState", &InputEventMouseButton::buttonsState)
            .addProperty("modifiers", &InputEventMouseButton::modifiers)
            .addProperty("button", &InputEventMouseButton::button)
            .addProperty("pressed", &InputEventMouseButton::pressed)
        .endClass()
        .beginClass<InputEvent>("InputEvent")
            .addProperty("type", &InputEvent::type)
            .addProperty("input_event_key", +[](const InputEvent*e) { return std::get<InputEventKey>(e->data);})
            .addProperty("input_event_mouse_motion", +[](const InputEvent*e) { return std::get<InputEventMouseMotion>(e->data);})
            .addProperty("input_event_mouse_button", +[](const InputEvent*e) { return std::get<InputEventMouseButton>(e->data);})
        .endClass()
        .beginNamespace("GamepadButton")
            .addVariable("A", GamepadButton::A)
            .addVariable("CROSS", GamepadButton::CROSS)
            .addVariable("B", GamepadButton::B)
            .addVariable("A", GamepadButton::A)
            .addVariable("CIRCLE", GamepadButton::CIRCLE)
            .addVariable("X ", GamepadButton::X )
            .addVariable("SQUARE", GamepadButton::SQUARE)
            .addVariable("Y", GamepadButton::Y)
            .addVariable("TRIANGLE", GamepadButton::TRIANGLE)
            .addVariable("LB", GamepadButton::LB)
            .addVariable("L1", GamepadButton::L1)
            .addVariable("RB", GamepadButton::RB)
            .addVariable("R1", GamepadButton::R1)
            .addVariable("BACK", GamepadButton::BACK)
            .addVariable("SHARE", GamepadButton::SHARE)
            .addVariable("START", GamepadButton::START)
            .addVariable("MENU", GamepadButton::MENU)
            .addVariable("RT", GamepadButton::RT)
            .addVariable("R2", GamepadButton::R2)
            .addVariable("DPAD_UP", GamepadButton::DPAD_UP)
            .addVariable("DPAD_RIGHT", GamepadButton::DPAD_RIGHT)
            .addVariable("DPAD_DOWN", GamepadButton::DPAD_DOWN)
            .addVariable("DPAD_LEFT", GamepadButton::DPAD_LEFT)
        .endNamespace()
        .beginNamespace("GamepadAxisJoystick")
           .addVariable("LEFT", GamepadAxisJoystick::LEFT)
           .addVariable("RIGHT", GamepadAxisJoystick::RIGHT)
        .endNamespace()
        .beginNamespace("GamepadAxis")
           .addVariable("LEFT_X", GamepadAxis::LEFT_X)
           .addVariable("LEFT_Y", GamepadAxis::LEFT_Y)
           .addVariable("RIGHT_X", GamepadAxis::RIGHT_X)
           .addVariable("RIGHT_Y", GamepadAxis::RIGHT_Y)
           .addVariable("LEFT_TRIGGER", GamepadAxis::LEFT_TRIGGER)
           .addVariable("RIGHT_TRIGGER", GamepadAxis::RIGHT_TRIGGER)
        .endNamespace()
        .beginClass<InputEventGamepadButton>("InputEventGamepadButton")
            .addProperty("button", &InputEventGamepadButton::button)
            .addProperty("pressed", &InputEventGamepadButton::pressed)
        .endClass()
        .beginClass<InputActionEntry>("InputActionEntry")
            .addProperty("type", &InputActionEntry::type)
            .addProperty("value", &InputActionEntry::value)
            .addProperty("pressed", &InputActionEntry::pressed)
        .endClass()
        .beginClass<InputAction>("InputAction")
            .addProperty("name", &InputAction::name)
            .addProperty("entries", &InputAction::entries)
        .endClass()
        .beginClass<Input>("Input")
            .addStaticFunction("is_key_pressed", &Input::isKeyPressed)
            .addStaticFunction("is_key_just_pressed", &Input::isKeyJustPressed)
            .addStaticFunction("is_key_just_released", &Input::isKeyJustReleased)
            .addStaticFunction("get_keyboard_vector", &Input::getKeyboardVector)
            .addStaticFunction("is_mouse_button_pressed", &Input::isMouseButtonPressed)
            .addStaticFunction("is_mouse_button_just_pressed", &Input::isMouseButtonJustPressed)
            .addStaticFunction("is_mouse_button_just_released", &Input::isMouseButtonJustReleased)
            .addStaticFunction("get_connected_joypads", &Input::getConnectedJoypads)
            .addStaticFunction("is_gamepad", &Input::isGamepad)
            .addStaticFunction("get_joypad_name", &Input::getJoypadName)
            .addStaticFunction("get_gamepad_vector", &Input::getGamepadVector)
            .addStaticFunction("is_gamepad_button_pressed", &Input::isGamepadButtonPressed)
            .addStaticFunction("is_gamepad_button_just_released", &Input::isGamepadButtonJustReleased)
            .addStaticFunction("is_gamepad_button_just_pressed", &Input::isGamepadButtonJustPressed)
            .addStaticFunction("add_action", &Input::addAction)
            .addStaticFunction("is_action", &Input::isAction)
        .endClass()

        .beginClass<Event>("Event")
            .addProperty("id", &Event::id)
            .addProperty("type", &Event::type)
            .addFunction("get_double", +[](const Event*e) { return std::any_cast<double>(e->payload);})
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
            .addFunction("unsubscribe", &EventManager::unsubscribe)
        .endClass()
        .beginClass<VirtualFS>("VirtualFS")
            .addFunction("get_path", &VirtualFS::getPath)
            .addFunction("file_exists", &VirtualFS::fileExists)
        .endClass()

        .beginNamespace("MouseMode")
           .addVariable("VISIBLE", MouseMode::VISIBLE)
           .addVariable("VISIBLE_CAPTURED", MouseMode::VISIBLE_CAPTURED)
           .addVariable("HIDDEN", MouseMode::HIDDEN)
           .addVariable("HIDDEN_CAPTURED", MouseMode::HIDDEN_CAPTURED)
        .endNamespace()
        .beginNamespace("MouseCursor")
           .addVariable("ARROW", MouseCursor::ARROW)
           .addVariable("WAIT", MouseCursor::WAIT)
           .addVariable("RESIZE_H", MouseCursor::RESIZE_H)
           .addVariable("RESIZE_V", MouseCursor::RESIZE_V)
        .endNamespace()
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
            .addFunction("set_mouse_mode", &RenderingWindow::setMouseMode)
            .addFunction("set_mouse_cursor", &RenderingWindow::setMouseCursor)
            .addFunction("reset_mouse_position", &RenderingWindow::resetMousePosition)
            .addFunction("get_mouse_position", &RenderingWindow::getMousePosition)
            .addFunction("set_mouse_position", &RenderingWindow::setMousePosition)
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
            .addFunction("add_view", &RenderTarget::addView)
            .addFunction("remove_view", &RenderTarget::removeView)
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
               luabridge::overload<const unique_id>(&ResourcesManager<Context, RenderTarget>::destroy),
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
            .addFunction("destroy",
                luabridge::overload<unique_id>(&ImageManager::destroy),
                luabridge::overload<const Image&>(&ImageManager::destroy)
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
              luabridge::nonConstOverload<const unique_id>(&ImageTextureManager::operator[]),
              luabridge::constOverload<const unique_id>(&ImageTextureManager::operator[])
              )
            .addFunction("destroy",
                  luabridge::overload<unique_id>(&ImageTextureManager::destroy),
                  luabridge::overload<const ImageTexture&>(&ImageTextureManager::destroy)
            )
        .endClass()

        .beginClass<Environment>("Environment")
           .addProperty("id", &Environment::id)
           .addProperty("color", &Environment::color)
           .addProperty("intensity", &Environment::intensity)
       .endClass()
       .beginClass<EnvironmentManager>("EnvironmentManager")
            .addFunction("create", +[](EnvironmentManager* self) -> Environment& {
                   return self->create();
            })
            .addFunction("get",
                luabridge::nonConstOverload<const unique_id>(&EnvironmentManager::operator[]),
                luabridge::constOverload<const unique_id>(&EnvironmentManager::operator[])
                )
            .addFunction("destroy",
                luabridge::overload<unique_id>(&EnvironmentManager::destroy),
                luabridge::overload<const Environment&>(&EnvironmentManager::destroy)
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
                .addFunction("destroy",
                     luabridge::overload<unique_id>(&MaterialManager::destroy),
                     luabridge::overload<const StandardMaterial&>(&MaterialManager::destroy),
                     luabridge::overload<const ShaderMaterial&>(&MaterialManager::destroy)
                )
        .endClass()

        .beginClass<AABB>("AABB")
            .addProperty("min", &AABB::min)
            .addProperty("max", &AABB::max)
            .addFunction("to_global", &AABB::toGlobal)
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
            .addProperty("aabb", &Mesh::getAABB)
        .endClass()
        .beginClass<MeshManager>("MeshManager")
            .addFunction("create",
                luabridge::overload<MeshManager*, const luabridge::LuaRef&, const luabridge::LuaRef&, const luabridge::LuaRef&>(
                    +[](MeshManager* self, const luabridge::LuaRef& v, const luabridge::LuaRef&i, const luabridge::LuaRef&s) -> Mesh& {
                        return self->create(v, i, s);
                    }),
                luabridge::overload<MeshManager*, const luabridge::LuaRef&, const luabridge::LuaRef&, const luabridge::LuaRef&, const std::string&>(
                    +[](MeshManager* self, const luabridge::LuaRef& v, const luabridge::LuaRef&i, const luabridge::LuaRef&s, const std::string&n) -> Mesh& {
                        return self->create(v, i, s, n);
                    })
             )
           .addFunction("get",
             luabridge::nonConstOverload<const unique_id>(&MeshManager::operator[]),
             luabridge::constOverload<const unique_id>(&MeshManager::operator[])
             )
            .addFunction("destroy",
               luabridge::overload<unique_id>(&MeshManager::destroy),
               luabridge::overload<const Mesh&>(&MeshManager::destroy)
            )
        .endClass()

        .beginClass<MeshInstance>("MeshInstance")
            .addProperty("id", &MeshInstance::id)
            .addProperty("mesh", &MeshInstance::getMesh)
            .addProperty("visible", &MeshInstance::isVisible, &MeshInstance::setVisible)
            .addProperty("cast_shadow", &MeshInstance::isCastShadows, &MeshInstance::setCastShadow)
            .addProperty("aabb", &MeshInstance::getAABB, &MeshInstance::setAABB)
            .addProperty("transform", &MeshInstance::getTransform, &MeshInstance::setTransform)
            .addFunction("get_surface_material", &MeshInstance::getSurfaceMaterial)
            .addFunction("set_surface_material_override", &MeshInstance::setSurfaceMaterialOverride)
            .addFunction("remove_surface_material_override", &MeshInstance::removeSurfaceMaterialOverride)
        .endClass()
        .beginClass<MeshInstanceManager>("MeshInstanceManager")
            .addFunction("create",
                luabridge::overload<MeshInstanceManager*, unique_id, bool, bool, const AABB&, const float4x4&>(
                    +[](MeshInstanceManager* self, unique_id mesh, bool visible, bool castShadow, const AABB& aabb, const float4x4& transform) -> MeshInstance& {
                        return self->create(mesh, visible, castShadow, aabb, transform);
                    }),
                luabridge::overload<MeshInstanceManager*, unique_id>(
                    +[](MeshInstanceManager* self, unique_id mesh) -> MeshInstance& {
                        return self->create(mesh);
                    })
             )
           .addFunction("get",
             luabridge::nonConstOverload<const unique_id>(&MeshInstanceManager::operator[]),
             luabridge::constOverload<const unique_id>(&MeshInstanceManager::operator[])
             )
            .addFunction("destroy",
               luabridge::overload<unique_id>(&MeshInstanceManager::destroy),
               luabridge::overload<const MeshInstance&>(&MeshInstanceManager::destroy)
            )
        .endClass()

        .beginClass<Camera>("Camera")
            .addProperty("id", &Camera::id)
            .addProperty("transform", &Camera::transform)
            .addProperty("projection", &Camera::projection)
        .endClass()
        .beginClass<CameraManager>("CameraManager")
          .addFunction("create",
              luabridge::overload<CameraManager*>(+[](CameraManager* self) -> Camera& {
                   return self->create();
              }),
              luabridge::overload<CameraManager*, const float4x4&, const float4x4&>(+[](
                    CameraManager* self,
                    const float4x4& transform,
                    const float4x4& projection) -> Camera& {
                   return self->create(transform, projection);
              })
             )
          .addFunction("get",
            luabridge::nonConstOverload<const unique_id>(&CameraManager::operator[]),
            luabridge::constOverload<const unique_id>(&CameraManager::operator[])
            )
            .addFunction("destroy",
                luabridge::overload<unique_id>(&CameraManager::destroy),
                luabridge::overload<const Camera&>(&CameraManager::destroy)
            )
        .endClass()

        .beginClass<RenderView>("RenderView")
            .addProperty("id", &RenderView::id)
            .addProperty("viewport", &RenderView::viewport)
            .addProperty("scissors", &RenderView::scissors)
            .addProperty("camera", &RenderView::camera)
            .addProperty("scene", &RenderView::scene)
        .endClass()
        .beginClass<RenderViewManager>("RenderViewManager")
          .addFunction("create",
              luabridge::overload<RenderViewManager*>(+[](RenderViewManager* self) -> RenderView& {
                   return self->create();
              }),
              luabridge::overload<RenderViewManager*, const vireo::Viewport&, const vireo::Rect&, const unique_id, const unique_id>(+[](
                    RenderViewManager* self,
                    const vireo::Viewport& viewport,
                    const vireo::Rect& scissors,
                    const unique_id camera,
                    const unique_id scene) -> RenderView& {
                   return self->create(viewport, scissors, camera, scene);
              })
             )
          .addFunction("get",
            luabridge::nonConstOverload<const unique_id>(&RenderViewManager::operator[]),
            luabridge::constOverload<const unique_id>(&RenderViewManager::operator[])
            )
            .addFunction("destroy",
                luabridge::overload<unique_id>(&RenderViewManager::destroy),
                luabridge::overload<const RenderView&>(&RenderViewManager::destroy)
            )
        .endClass()

        .beginClass<Scene>("Scene")
            .addProperty("id", &Scene::id)
            .addProperty("environment", &Scene::getEnvironment, &Scene::setEnvironment)
            .addFunction("add_instance", &Scene::addInstance)
            .addFunction("update_instance", &Scene::updateInstance)
            .addFunction("remove_instance", &Scene::removeInstance)
        .endClass()
        .beginClass<SceneManager>("SceneManager")
           .addFunction("create", +[](SceneManager* self) -> Scene& {
                    return self->create();
            })
           .addFunction("get",
             luabridge::nonConstOverload<const unique_id>(&SceneManager::operator[]),
             luabridge::constOverload<const unique_id>(&SceneManager::operator[])
             )
            .addFunction("destroy",
                luabridge::overload<unique_id>(&SceneManager::destroy),
                luabridge::overload<const Scene&>(&SceneManager::destroy)
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
            .addProperty("render_view_manager",
                +[](const ResourcesRegistry* rl) -> RenderViewManager& {
                    return rl->get<RenderViewManager>();
            })
            .addProperty("rendering_window_manager",
                +[](const ResourcesRegistry* rl) -> RenderingWindowManager& {
                return rl->get<RenderingWindowManager>();
            })
            .addProperty("camera_manager",
                +[](const ResourcesRegistry* rl) -> CameraManager& {
                return rl->get<CameraManager>();
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
            .addProperty("mesh_instance_manager",
                +[](const ResourcesRegistry* rl) -> MeshInstanceManager& {
                return rl->get<MeshInstanceManager>();
            })
            .addProperty("scene_manager",
                +[](const ResourcesRegistry* rl) -> SceneManager& {
                return rl->get<SceneManager>();
            })
            .addProperty("environment_manager",
                   +[](const ResourcesRegistry* rl) -> EnvironmentManager& {
                   return rl->get<EnvironmentManager>();
               })
        .endClass()

        .beginClass<Context>("Context")
           .addProperty("exit", [this](Context*) { return &ctx.exit;})
           .addProperty("vireo", [this](Context*) { return ctx.vireo;})
           .addProperty("fs",  [this](Context*) { return &ctx.fs;})
           .addProperty("events", [this](Context*) { return &ctx.events;})
           .addProperty("res", [this](Context*) { return &ctx.res;})
           .addProperty("graphic_queue", [this](Context*) { return ctx.graphicQueue;})
       .endClass()

        .endNamespace();

    }
}