/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.ecs.systems;

export import lysa.ecs.components;
export import lysa.ecs.flecs;

export namespace lysa::ecs {

    class TransformModule {
    public:
        TransformModule(const flecs::world& w);
    private:
        static void updateGlobalTransform(flecs::entity e, Transform& t);
    };

    class MeshInstanceModule {
    public:
        MeshInstanceModule(const flecs::world& w);
        void disable() const;
    private:
        flecs::observer observerTransform;
        flecs::observer observerVisible;
    };

    struct RenderModule {
        RenderModule(const flecs::world& w);
    };

    class Modules {
    public:
        Modules(flecs::world& w);
        ~Modules();
    private:
        flecs::entity renderModule;
        flecs::entity meshInstanceModule;
        flecs::entity transformModule;
    };

}
