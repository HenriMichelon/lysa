---@diagnostic disable: missing-return
--EmmyLua annotations and documentation for Lysa

return {
    ---@class ecs.RenderTarget
    ---@field render_target integer
    RenderTarget = ecs.RenderTarget,

    ---@class Viewport
    ---@field viewport vireo::Viewport
    ---@field scissors vireoRect
    Viewport = ecs.Viewport,

    ---@class Camera
    ---@field is_perspective bool
    ---@field fov float
    ---@field aspect_ratio float
    ---@field near float
    ---@field far float
    ---@field left float
    ---@field right float
    ---@field top float
    ---@field bottom float
    Camera = ecs.Camera,

    ---@class CameraRef
    ---@field camera ecs::entity
    CameraRef = ecs.CameraRef,

    ---@class MaterialOverride
    ---@field mesh integer
    MaterialOverride = ecs.MaterialOverride,

    ---@class Scene
    ---@field scene integer
    Scene = ecs.Scene,

    ---@class SceneRef
    ---@field scene ecs.Scene
    SceneRef = ecs.SceneRef,

    ---@class MeshInstance
    ---@field surface_index integer
    ---@field material integer
    MeshInstance = ecs.MeshInstance,

    ---@class Visible
    Visible = ecs.Visible,

    ---@class AmbientLight
    ---@field color lysa::float3
    ---@field intensity float
    AmbientLight = ecs.AmbientLight,

    ---@class Transform
    ---@field local lysa::float4x4
    ---@field global lysa::float4x4
    Transform = ecs.Transform,

    ---@class SceneRef
    ---@field scene ecs.Scene
    SceneRef = ecs.SceneRef,

    ---@class ecs.entity
    ---@field is_alive boolean
    ---@field destruct fun(self:ecs.entity):nil
    ---@field render_target  ecs.RenderTarget
    entity = ecs.entity,

  ---@overload fun(e:ecs.entity, p:lysa.float3)
  ---@overload fun(e:ecs.entity, x:float,y:float,z:float)
    set_position = ecs.set_position,

    ---@class ecs.world
    ---@field entity fun(self:ecs.world):ecs.entity
    world = ecs.world,
}
