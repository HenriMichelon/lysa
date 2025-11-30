---@diagnostic disable: missing-return
--EmmyLua annotations and documentation for Lysa

return {
    ------------------------------------------------------------------------
    -- flecs::world
    ------------------------------------------------------------------------

    ---@class flecs.world
    ---@field entity fun(self:flecs.world):flecs.entity
    world = flecs.world,

    ------------------------------------------------------------------------
    -- flecs::entity
    ------------------------------------------------------------------------

    ---@class flecs.entity
    ---@field is_alive boolean
    ---@field destruct fun(self:flecs.entity):nil
    entity = flecs.entity,
}
