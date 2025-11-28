#
# Copyright (c) 2025-present Henri Michelon
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.29)

#######################################################
project(lysa)
if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.env.cmake")
    message(FATAL_ERROR "Please create a .env.cmake file with the VIREO_PROJECT_DIR variable")
endif ()
include(.env.cmake)
if (NOT DEFINED VIREO_PROJECT_DIR)
    message(FATAL_ERROR "Please set VIREO_PROJECT_DIR in the .env.cmake file")
endif()

#######################################################
set(LYSA_TARGET ${PROJECT_NAME})
set(DIRECTX_BACKEND ON)
set(LUA_BINDINGS ON)
set(PHYSIC_ENGINE_JOLT ON)
set(PHYSIC_ENGINE_PHYSX OFF)
set(FORWARD_RENDERER ON)
set(DEFERRED_RENDERER OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

#######################################################
if (LUA_BINDINGS)
    add_compile_definitions(LUA_BINDINGS)
endif ()

#######################################################
if(NOT FORWARD_RENDERER AND NOT DEFERRED_RENDERER)
    message(FATAL_ERROR "Please activate a renderer with FORWARD_RENDERER or DEFERRED_RENDERER")
endif()
if (FORWARD_RENDERER)
    add_compile_definitions(FORWARD_RENDERER)
endif ()
if (DEFERRED_RENDERER)
    add_compile_definitions(DEFERRED_RENDERER)
endif ()


#######################################################
if(WIN32)
    if(MSVC)
        set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF)
    endif()
    add_compile_definitions(UNICODE _UNICODE)
endif ()

#######################################################
if(NOT PHYSIC_ENGINE_JOLT AND NOT PHYSIC_ENGINE_PHYSX)
    message(FATAL_ERROR "Please activate a physics library with PHYSIC_ENGINE_JOLT or PHYSIC_ENGINE_PHYSX")
endif()
if(PHYSIC_ENGINE_JOLT AND PHYSIC_ENGINE_PHYSX)
    message(FATAL_ERROR "Please deactivate a physics library")
endif()
if(PHYSIC_ENGINE_JOLT)
    add_compile_definitions(PHYSIC_ENGINE_JOLT)
    add_compile_definitions(JPH_DISABLE_CUSTOM_ALLOCATOR)
    #add_compile_definitions(JPH_USE_STD_VECTOR)
endif()
if (PHYSIC_ENGINE_PHYSX)
    if (NOT DEFINED PHYSX_ROOT)
        message(FATAL_ERROR "Please set PHYSX_ROOT in the .env.cmake file")
    endif()
    if (NOT DEFINED PHYSX_LIBRARIES)
        message(FATAL_ERROR "Please set PHYSX_LIBRARIES in the .env.cmake file")
    endif()
    add_compile_definitions(PHYSIC_ENGINE_PHYSX PX_PHYSX_STATIC_LIB PX_FOUNDATION_STATIC_LIB PX_PHYSX_COMMON_STATIC_LIB)
endif()

#######################################################
include(FetchContent)
include(cmake/libraries.cmake)
include(cmake/shaders.cmake)

#######################################################
add_subdirectory(${VIREO_PROJECT_DIR} external_lib_build)
set(VIREO_TARGET "vireo_rhi")

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(ENGINE_SRC_DIR ${SRC_DIR}/${LYSA_TARGET})
set(SHADERS_SRC_DIR ${ENGINE_SRC_DIR}/shaders)
set(SHADERS_INCLUDE_DIR ${ENGINE_SRC_DIR}/shaders/include)
set(SHADERS_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(DEPENDS_SRC_DIR ${ENGINE_SRC_DIR}/depends)

set(HLSLPP_TARGET hlslpp)
set(HLSLPP_SRC_DIR ${DEPENDS_SRC_DIR}/${HLSLPP_TARGET})

#######################################################
# Slang shaders
file(MAKE_DIRECTORY ${SHADERS_BUILD_DIR})
set(SHADERS_SOURCE_FILES
        "${SHADERS_SRC_DIR}/default.vert.slang"
        "${SHADERS_SRC_DIR}/depth_prepass.vert.slang"
        "${SHADERS_SRC_DIR}/frustum_culling.comp.slang"
        "${SHADERS_SRC_DIR}/frustum_culling_shadowmap.comp.slang"
        "${SHADERS_SRC_DIR}/quad.vert.slang"
        "${SHADERS_SRC_DIR}/vector.slang"
        "${SHADERS_SRC_DIR}/vector_ui.slang"
        "${SHADERS_SRC_DIR}/glyph.slang"
        "${SHADERS_SRC_DIR}/glyph_ui.slang"
        "${SHADERS_SRC_DIR}/deferred/gbuffers.frag.slang"
        "${SHADERS_SRC_DIR}/deferred/glighting.frag.slang"
        "${SHADERS_SRC_DIR}/deferred/glighting_bloom.frag.slang"
        "${SHADERS_SRC_DIR}/deferred/ssao.frag.slang"
        "${SHADERS_SRC_DIR}/forward/forward.frag.slang"
        "${SHADERS_SRC_DIR}/forward/forward_bloom.frag.slang"
        "${SHADERS_SRC_DIR}/forward/transparency_oit.frag.slang"
        "${SHADERS_SRC_DIR}/forward/transparency_oit_composite.frag.slang"
        "${SHADERS_SRC_DIR}/postprocess/greyscale.frag.slang"
        "${SHADERS_SRC_DIR}/postprocess/bloom_blur.frag.slang"
        "${SHADERS_SRC_DIR}/postprocess/ssao_blur.frag.slang"
        "${SHADERS_SRC_DIR}/postprocess/fxaa.frag.slang"
        "${SHADERS_SRC_DIR}/postprocess/smaa_edge_detect.frag.slang"
        "${SHADERS_SRC_DIR}/postprocess/smaa_blend_weight.frag.slang"
        "${SHADERS_SRC_DIR}/postprocess/smaa_neighborhood_blend.frag.slang"
        "${SHADERS_SRC_DIR}/postprocess/gamma_correction.frag.slang"
        "${SHADERS_SRC_DIR}/postprocess/reinhard.frag.slang"
        "${SHADERS_SRC_DIR}/postprocess/aces.frag.slang"
        "${SHADERS_SRC_DIR}/shadows/shadowmap.vert.slang"
        "${SHADERS_SRC_DIR}/shadows/shadowmap.frag.slang"
        "${SHADERS_SRC_DIR}/shadows/shadowmap_cubemap.frag.slang"
)
add_shaders(${LYSA_TARGET}_shaders ${SHADERS_BUILD_DIR} ${SHADERS_INCLUDE_DIR} ${SHADERS_SOURCE_FILES})

#######################################################
# xxHash library
add_library(xxhash STATIC
        ${xxhash_SOURCE_DIR}/xxhash.c
)
target_include_directories(xxhash PUBLIC
        ${xxhash_SOURCE_DIR}
)
compile_options(xxhash)

#######################################################
if(WIN32)
    set(OS_SRC
            ${ENGINE_SRC_DIR}/os/win32/Main.cpp
            ${ENGINE_SRC_DIR}/os/win32/RenderingWindow.cpp
    )
    set(OS_MODULES ""
    )
endif ()

#######################################################
if(PHYSIC_ENGINE_JOLT)
    set(PHYSICS_SRC

    )
    set(PHYSICS_MODULES
    )
endif ()
if(PHYSIC_ENGINE_PHYSX)
    set(PHYSICS_SRC
    )
    set(PHYSICS_MODULES
    )
endif ()

#######################################################
if(LUA_BINDINGS)
    set(LUA_BINDINGS_SRC
            ${ENGINE_SRC_DIR}/bindings/lua/Lua.cpp
            ${lua_socket_SOURCE_DIR}/src/luasocket.h
            ${lua_socket_SOURCE_DIR}/src/except.c
            ${lua_socket_SOURCE_DIR}/src/luasocket.c
            ${lua_socket_SOURCE_DIR}/src/timeout.c
            ${lua_socket_SOURCE_DIR}/src/buffer.c
            ${lua_socket_SOURCE_DIR}/src/io.c
            ${lua_socket_SOURCE_DIR}/src/auxiliar.c
            ${lua_socket_SOURCE_DIR}/src/options.c
            ${lua_socket_SOURCE_DIR}/src/inet.c
            ${lua_socket_SOURCE_DIR}/src/tcp.c
            ${lua_socket_SOURCE_DIR}/src/udp.c
            ${lua_socket_SOURCE_DIR}/src/select.c
            ${lua_socket_SOURCE_DIR}/src/wsocket.c
    )
    set(LUA_BINDINGS_MODULES
            ${ENGINE_SRC_DIR}/bindings/lua/Lua.ixx
    )
endif ()

#######################################################
if(FORWARD_RENDERER)
    set(FORWARD_RENDERER_SRC
            ${ENGINE_SRC_DIR}/renderers/ForwardRenderer.cpp
            ${ENGINE_SRC_DIR}/renderers/renderpasses/ForwardColor.cpp
    )
    set(FORWARD_RENDERER_MODULES
            ${ENGINE_SRC_DIR}/renderers/ForwardRenderer.ixx
            ${ENGINE_SRC_DIR}/renderers/renderpasses/ForwardColor.ixx
    )
endif ()
if(DEFERRED_RENDERER)
    set(DEFERRED_RENDERER_SRC
    )
    set(DEFERRED_RENDERER_MODULES
    )
endif ()

#######################################################
add_library(${LYSA_TARGET} STATIC
        ${ENGINE_SRC_DIR}/Context.cpp
        ${ENGINE_SRC_DIR}/Event.cpp
        ${ENGINE_SRC_DIR}/Global.cpp
        ${ENGINE_SRC_DIR}/Log.cpp
        ${ENGINE_SRC_DIR}/Lysa.cpp
        ${ENGINE_SRC_DIR}/Math.cpp
        ${ENGINE_SRC_DIR}/VirtualFS.cpp

        ${DEFERRED_RENDERER_SRC}
        ${FORWARD_RENDERER_SRC}
        ${ENGINE_SRC_DIR}/renderers/Renderer.cpp
        ${ENGINE_SRC_DIR}/renderers/renderpasses/Renderpass.cpp

        ${ENGINE_SRC_DIR}/resources/RenderTarget.cpp
        ${ENGINE_SRC_DIR}/resources/RenderingWindow.cpp
        ${ENGINE_SRC_DIR}/resources/Viewport.cpp

        ${OS_SRC}
        ${PHYSICS_SRC}
        ${LUA_BINDINGS_SRC}
        ${DEPENDS_SRC_DIR}/stb/stb.cpp
)
target_sources(${LYSA_TARGET}
    PUBLIC
    FILE_SET CXX_MODULES
    FILES
        ${ENGINE_SRC_DIR}/Context.ixx
        ${ENGINE_SRC_DIR}/Event.ixx
        ${ENGINE_SRC_DIR}/Exception.ixx
        ${ENGINE_SRC_DIR}/Global.ixx
        ${ENGINE_SRC_DIR}/Log.ixx
        ${ENGINE_SRC_DIR}/Lysa.ixx
        ${ENGINE_SRC_DIR}/Math.ixx
        ${ENGINE_SRC_DIR}/Types.ixx
        ${ENGINE_SRC_DIR}/VirtualFS.ixx

        ${FORWARD_RENDERER_MODULES}
        ${DEFERRED_RENDERER_MODULES}
        ${ENGINE_SRC_DIR}/renderers/Renderer.ixx
        ${ENGINE_SRC_DIR}/renderers/renderpasses/Renderpass.ixx

        ${ENGINE_SRC_DIR}/resources/Manager.ixx
        ${ENGINE_SRC_DIR}/resources/RenderingWindow.ixx
        ${ENGINE_SRC_DIR}/resources/RenderTarget.ixx
        ${ENGINE_SRC_DIR}/resources/ResourcesLocator.ixx
        ${ENGINE_SRC_DIR}/resources/ResourcesManager.ixx
        ${ENGINE_SRC_DIR}/resources/Viewport.ixx

        ${OS_MODULES}
        ${PHYSICS_MODULES}
        ${LUA_BINDINGS_MODULES}
)

compile_options(${LYSA_TARGET})
target_include_directories(${LYSA_TARGET} PUBLIC
        ${INCLUDE_DIR}
        ${HLSLPP_SRC_DIR}/include
        ${DEPENDS_SRC_DIR}/stb
        ${xxhash_SOURCE_DIR}
        ${DEPENDS_SRC_DIR}/json
        ${DEPENDS_SRC_DIR}
        ${freetype_SOURCE_DIR}/include
        ${harfbuzz_SOURCE_DIR}/src
        ${msdfgen_SOURCE_DIR}
        ${msdf_atlas_gen_SOURCE_DIR}
        ${lua_cmake_SOURCE_DIR}/upstream
)
target_link_libraries(${LYSA_TARGET} ${VIREO_TARGET}
        std-cxx-modules
        xxhash
        Freetype::Freetype
        harfbuzz
)
if(LUA_BINDINGS)
    target_link_libraries(${LYSA_TARGET} ${VIREO_TARGET}
        lua
    )
endif ()
add_dependencies(${LYSA_TARGET} ${VIREO_TARGET})
target_precompile_headers(${LYSA_TARGET} PRIVATE
    "<intrin.h>"
)
set_property(TARGET ${LYSA_TARGET} PROPERTY COMPILE_WARNING_AS_ERROR ON)

#######################################################
if(WIN32)
    target_compile_definitions(${LYSA_TARGET} PRIVATE WIN32_LEAN_AND_MEAN)
    target_link_libraries(${LYSA_TARGET} Ws2_32)
    if (DIRECTX_BACKEND)
        target_link_libraries(${LYSA_TARGET} d3d12)
    endif ()
    if(MINGW)
        target_link_options(${LYSA_TARGET} PRIVATE "-mwindows")
    endif()
    set_target_properties(${LYSA_TARGET} PROPERTIES
            WIN32_EXECUTABLE TRUE)
endif()

#######################################################
if(PHYSIC_ENGINE_JOLT)
    target_include_directories(${LYSA_TARGET} PUBLIC ${JoltPhysics_SOURCE_DIR}/..)
    target_link_libraries(${LYSA_TARGET} Jolt)
endif()
if (PHYSIC_ENGINE_PHYSX)
    target_include_directories(${LYSA_TARGET} PUBLIC
        ${PHYSX_ROOT}/include
    )
    target_link_directories(${LYSA_TARGET} PUBLIC ${PHYSX_LIBRARIES})
    target_link_libraries(${LYSA_TARGET}
        PhysX_static_64
        PhysXCharacterKinematic_static_64
        PhysXCommon_static_64
        PhysXCooking_static_64
        PhysXExtensions_static_64
        PhysXFoundation_static_64
        PhysXPvdSDK_static_64
    )
endif()


#######################################################
find_program(DOXYPRESS_EXECUTABLE doxypress)

if (DOXYPRESS_EXECUTABLE)
    message(STATUS "DoxyPress found: ${DOXYPRESS_EXECUTABLE}")
    set(DOCS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/docs")

    add_custom_target(lysa_docs
            COMMAND ${DOXYPRESS_EXECUTABLE}
            COMMAND ${CMAKE_COMMAND} -E copy
            ${DOCS_DIRECTORY}/navtree.js
            ${DOCS_DIRECTORY}/html/navtree.js
            WORKING_DIRECTORY ${DOCS_DIRECTORY}
            COMMENT "Generating Lysa documentation with DoxyPress"
            VERBATIM
    )
else()
    message(STATUS "DoxyPress not found, doxypress target will not be available.")
endif()