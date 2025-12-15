local vireo = require('vireo')
local flecs = require('flecs')

---@diagnostic disable: missing-return
--EmmyLua annotations and documentation for Lysa

return {
    ------------------------------------------------------------------------
    -- Math types
    ------------------------------------------------------------------------

    ---@class lysa.float2
    ---@field x number
    ---@field y number
    ---@field r number
    ---@field g number
    float2 = lysa.float2,

    ---@class lysa.float3
    ---@field x number
    ---@field y number
    ---@field z number
    ---@field r number
    ---@field g number
    ---@field b number
    float3 = lysa.float3,

    ---@class lysa.float4
    ---@field x number
    ---@field y number
    ---@field z number
    ---@field w number
    ---@field r number
    ---@field g number
    ---@field b number
    ---@field a number
    float4 = lysa.float4,

    ---@class lysa.quaternion
    ---@field x number
    ---@field y number
    ---@field z number
    ---@field w number
    ---@field r number
    ---@field g number
    ---@field b number
    ---@field a number
    quaternion = lysa.quaternion,

    ------------------------------------------------------------------------
    -- Free functions
    ------------------------------------------------------------------------

    ---@param q lysa.quaternion
    ---@return lysa.float3
    euler_angles = lysa.euler_angles,

    ---@param v number
    ---@return number
    radians = lysa.radians,

    ---@overload fun(a:number, b:number):boolean
    ---@overload fun(a:lysa.quaternion, b:lysa.quaternion):boolean
    almost_equals = lysa.almost_equals,

    ---@param eye lysa.float3
    ---@param center lysa.float3
    ---@param up lysa.float3
    ---@return any  @matrix (float4x4)
    look_at = lysa.look_at,

    ---@param fovy number
    ---@param aspect number
    ---@param znear number
    ---@param zfar number
    ---@return any  @matrix (float4x4)
    perspective = lysa.perspective,

    ---@param left number
    ---@param right number
    ---@param bottom number
    ---@param top number
    ---@param znear number
    ---@param zfar number
    ---@return any  @matrix (float4x4)
    orthographic = lysa.orthographic,

    ---@param min integer
    ---@param max integer
    ---@return integer
    randomi = lysa.randomi,

    ---@param min number
    ---@param max number
    ---@return number
    randomf = lysa.randomf,

    ---@return boolean
    is_windows = lysa.is_windows,

    ---@return integer  @milliseconds
    get_current_time_milliseconds = lysa.get_current_time_milliseconds,

    ---@param name string
    ---@return string
    sanitize_name = lysa.sanitize_name,

    ---@param path string
    ---@return boolean
    dir_exists = lysa.dir_exists,

    ---@param v any
    ---@return lysa.float3
    to_float3 = lysa.to_float3,

    ---@param v any
    ---@return lysa.float4
    to_float4 = lysa.to_float4,

    ---@param s string
    ---@return string
    to_lower = lysa.to_lower,

    ------------------------------------------------------------------------
    -- Log
    ------------------------------------------------------------------------

    ---@class lysa.Log
    ---@field log fun(msg:string)
    ---@field debug fun(msg:string)
    ---@field info fun(msg:string)
    ---@field game1 fun(msg:string)
    ---@field game2 fun(msg:string)
    ---@field game3 fun(msg:string)
    ---@field warning fun(msg:string)
    ---@field error fun(msg:string)
    ---@field critical fun(msg:string)
    Log = lysa.Log,

    ------------------------------------------------------------------------
    -- Events
    ------------------------------------------------------------------------

    ---@class lysa.Event
    ---@field id integer
    ---@field type integer
    Event = lysa.Event,

    ---@class lysa.EventManager
    ---@field push fun(self:lysa.EventManager, e:lysa.Event):nil
    ---@field subscribe fun(self:lysa.EventManager, type:any, id:integer, cb:function):nil
    EventManager = lysa.EventManager,

    ------------------------------------------------------------------------
    -- VirtualFS
    ------------------------------------------------------------------------

    ---@class lysa.VirtualFS
    ---@field get_path fun(self:lysa.VirtualFS, p:string):string
    ---@field file_exists fun(self:lysa.VirtualFS, p:string):boolean
    VirtualFS = lysa.VirtualFS,

    ------------------------------------------------------------------------
    -- Rendering windows
    ------------------------------------------------------------------------

    ---@class lysa.RenderingWindowMode
    ---@field WINDOWED integer
    ---@field WINDOWED_MAXIMIZED integer
    ---@field WINDOWED_FULLSCREEN integer
    ---@field FULLSCREEN integer
    RenderingWindowMode = lysa.RenderingWindowMode,

    ---@class lysa.RenderingWindowEventType
    ---@field READY integer
    ---@field CLOSING integer
    ---@field RESIZED integer
    RenderingWindowEventType = lysa.RenderingWindowEventType,

    ---@class lysa.RenderingWindowEvent
    ---@field id integer
    ---@field type integer
    RenderingWindowEvent = lysa.RenderingWindowEvent,

    ---@class lysa.RenderingWindowConfiguration
    ---@field title string
    ---@field mode integer  @lysa.RenderingWindowMode
    ---@field x integer
    ---@field y integer
    ---@field width integer
    ---@field height integer
    ---@field monitor integer
    RenderingWindowConfiguration = lysa.RenderingWindowConfiguration,

    ---@class lysa.RenderingWindow
    ---@field id integer
    ---@field x integer        @read-only (getter)
    ---@field y integer        @read-only (getter)
    ---@field width integer    @read-only (getter)
    ---@field height integer   @read-only (getter)
    ---@field stopped boolean  @read-only (getter)
    ---@field platform_handle lightuserdata
    ---@field show fun(self:lysa.RenderingWindow):nil
    ---@field close fun(self:lysa.RenderingWindow):nil
    RenderingWindow = lysa.RenderingWindow,

    ---@class lysa.RenderingWindowManager
    ---@field ID integer
    ---@field create fun(self:lysa.RenderingWindowManager, cfg:lysa.RenderingWindowConfiguration):lysa.RenderingWindow
    ---@field get fun(self:lysa.RenderingWindowManager, id:integer):lysa.RenderingWindow
    RenderingWindowManager = lysa.RenderingWindowManager,

    ------------------------------------------------------------------------
    -- Render targets
    ------------------------------------------------------------------------

    ---@class lysa.RenderTargetConfiguration
    ---@field rendering_window_handle lightuserdata|lysa.RenderingWindow
    ---@field renderer_configuration lysa.RendererConfiguration
    RenderTargetConfiguration = lysa.RenderTargetConfiguration,

    ---@class lysa.RenderTargetEventType
    ---@field PAUSED integer
    ---@field RESUMED integer
    ---@field RESIZED integer
    RenderTargetEventType = lysa.RenderTargetEventType,

    ---@class lysa.RenderTargetEvent
    ---@field id integer
    ---@field type integer
    RenderTargetEvent = lysa.RenderTargetEvent,

    ---@class lysa.RenderTarget
    ---@field id integer
    ---@field pause fun(self:lysa.RenderTarget, paused:boolean|nil):nil
    ---@field swap_chain fun(self:lysa.RenderTarget):vireo.SwapChain|nil
    ---@field rendering_window_handle fun(self:lysa.RenderTarget):lightuserdata|lysa.RenderingWindow
    RenderTarget = lysa.RenderTarget,

    ---@class lysa.RenderTargetManager
    ---@field ID integer
    ---@field create fun(self:lysa.RenderTargetManager, cfg:lysa.RenderTargetConfiguration):lysa.RenderTarget
    ---@field get fun(self:lysa.RenderTargetManager, id:integer):lysa.RenderTarget
    ---@field destroy fun(self:lysa.RenderTargetManager, idOrPtr:any):nil
    RenderTargetManager = lysa.RenderTargetManager,

    ------------------------------------------------------------------------
    -- Render passes / renderer
    ------------------------------------------------------------------------

    ---@class lysa.Renderpass
    Renderpass = lysa.Renderpass,

    ---@class lysa.RendererType
    ---@field FORWARD integer
    ---@field DEFERRED integer
    RendererType = lysa.RendererType,

    ---@class lysa.RendererConfiguration
    ---@field renderer_type integer        @lysa.RendererType
    ---@field swap_chain_format vireo.ImageFormat
    ---@field present_mode vireo.PresentMode
    ---@field frames_in_flight integer
    ---@field color_rendering_format vireo.ImageFormat
    ---@field depth_stencil_format vireo.ImageFormat
    ---@field clear_color lysa.float4
    ---@field msaa vireo.MSAA
    RendererConfiguration = lysa.RendererConfiguration,

    ---@class lysa.Renderer
    Renderer = lysa.Renderer,

    ------------------------------------------------------------------------
    -- Resources locator
    ------------------------------------------------------------------------

    ---@class lysa.ResourcesRegistry
    ---@field get fun(self:lysa.ResourcesRegistry, id:integer):any
    ---@field render_target_manager lysa.RenderTargetManager
    ---@field viewport_manager lysa.ViewportManager
    ---@field rendering_window_manager lysa.RenderingWindowManager
    ResourcesRegistry = lysa.ResourcesRegistry,

    ------------------------------------------------------------------------
    -- Context
    ------------------------------------------------------------------------

    ---@class lysa.Context
    ---@field exit boolean
    ---@field vireo vireo.Vireo
    ---@field fs lysa.VirtualFS
    ---@field events lysa.EventManager
    ---@field world flecs.world
    ---@field resources lysa.ResourcesRegistry
    ---@field graphic_queue vireo.SubmitQueue
    Context = lysa.Context,

    ---@class lysa.Context
    ctx = lysa.ctx,
}
