-- ZeroBrane Studio API autocomplete for Lysa

local M = {}

local function deepmerge(target, source)
    for k, v in pairs(source) do
        local tv = target[k]

        if type(v) == "table" then
            if type(tv) == "table" then
                deepmerge(tv, v)
            else
                local t = {}
                deepmerge(t, v)
                target[k] = t
            end
        else
            target[k] = v
        end
    end
end

deepmerge(M, require('./api/lua/lysa/context'))
deepmerge(M, require('./api/lua/lysa/event'))
deepmerge(M, require('./api/lua/lysa/rendering_window'))

return M