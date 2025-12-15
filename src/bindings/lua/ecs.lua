---@diagnostic disable: missing-return
--EmmyLua annotations and documentation for Lysa

return {
    ---@class ecs.RenderTarget
    ---@field render_target integer
    RenderTarget = ecs.RenderTarget,


    ---@class ecs.entity
    ---@field is_alive boolean
    ---@field destruct fun(self:ecs.entity):nil
    ---@field render_target  ecs.RenderTarget
    entity = ecs.entity,


    ---@class ecs.world
    ---@field entity fun(self:ecs.world):ecs.entity
    world = ecs.world,
}
