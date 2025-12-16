local vireo = require('vireo')
local ecs = require('ecs')

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

    ---@class Key
    ---@field KEY_NONE integer
    ---@field KEY_SPACE integer
    ---@field KEY_DASH integer
    ---@field KEY_PIPE integer
    ---@field KEY_APOSTROPHE integer
    ---@field KEY_COMMA integer
    ---@field KEY_PERIOD integer
    ---@field KEY_QUESTIONMARK integer
    ---@field KEY_0 integer
    ---@field KEY_1 integer
    ---@field KEY_2 integer
    ---@field KEY_3 integer
    ---@field KEY_4 integer
    ---@field KEY_5 integer
    ---@field KEY_6 integer
    ---@field KEY_7 integer
    ---@field KEY_8 integer
    ---@field KEY_9 integer
    ---@field KEY_SEMICOLON integer
    ---@field KEY_EQUAL integer
    ---@field KEY_A integer
    ---@field KEY_B integer
    ---@field KEY_C integer
    ---@field KEY_D integer
    ---@field KEY_E integer
    ---@field KEY_F integer
    ---@field KEY_G integer
    ---@field KEY_H integer
    ---@field KEY_I integer
    ---@field KEY_J integer
    ---@field KEY_K integer
    ---@field KEY_L integer
    ---@field KEY_M integer
    ---@field KEY_N integer
    ---@field KEY_O integer
    ---@field KEY_P integer
    ---@field KEY_Q integer
    ---@field KEY_R integer
    ---@field KEY_S integer
    ---@field KEY_T integer
    ---@field KEY_U integer
    ---@field KEY_V integer
    ---@field KEY_W integer
    ---@field KEY_X integer
    ---@field KEY_Y integer
    ---@field KEY_Z integer
    ---@field KEY_LEFT_BRACKET integer
    ---@field KEY_BACKSLASH integer
    ---@field KEY_RIGHT_BRACKET integer
    ---@field KEY_GRAVE_ACCENT integer
    ---@field KEY_ESCAPE integer
    ---@field KEY_ENTER integer
    ---@field KEY_TAB integer
    ---@field KEY_BACKSPACE integer
    ---@field KEY_INSERT integer
    ---@field KEY_DELETE integer
    ---@field KEY_RIGHT integer
    ---@field KEY_LEFT integer
    ---@field KEY_DOWN integer
    ---@field KEY_UP integer
    ---@field KEY_PAGE_UP integer
    ---@field KEY_PAGE_DOWN integer
    ---@field KEY_HOME integer
    ---@field KEY_END integer
    ---@field KEY_CAPS_LOCK integer
    ---@field KEY_SCROLL_LOCK integer
    ---@field KEY_NUM_LOCK integer
    ---@field KEY_PRINT_SCREEN integer
    ---@field KEY_PAUSE integer
    ---@field KEY_F1 integer
    ---@field KEY_F2 integer
    ---@field KEY_F3 integer
    ---@field KEY_F4 integer
    ---@field KEY_F5 integer
    ---@field KEY_F6 integer
    ---@field KEY_F7 integer
    ---@field KEY_F8 integer
    ---@field KEY_F9 integer
    ---@field KEY_F10 integer
    ---@field KEY_F11 integer
    ---@field KEY_F12 integer
    ---@field KEY_KP_0 integer
    ---@field KEY_KP_1 integer
    ---@field KEY_KP_2 integer
    ---@field KEY_KP_3 integer
    ---@field KEY_KP_4 integer
    ---@field KEY_KP_5 integer
    ---@field KEY_KP_6 integer
    ---@field KEY_KP_7 integer
    ---@field KEY_KP_8 integer
    ---@field KEY_KP_9 integer
    ---@field KEY_KP_PERIOD integer
    ---@field KEY_KP_DIVIDE integer
    ---@field KEY_KP_MULTIPLY integer
    ---@field KEY_KP_SUBTRACT integer
    ---@field KEY_KP_ADD integer
    ---@field KEY_KP_ENTER integer
    ---@field KEY_KP_EQUAL integer
    ---@field KEY_LEFT_SHIFT integer
    ---@field KEY_LEFT_CONTROL integer
    ---@field KEY_LEFT_ALT integer
    ---@field KEY_LEFT_SUPER integer
    ---@field KEY_RIGHT_SHIFT integer
    ---@field KEY_RIGHT_CONTROL integer
    ---@field KEY_RIGHT_ALT integer
    ---@field KEY_RIGHT_SUPER integer
    Key = lysa.Key,

    ---@class lysa.InputEventType
    ---@field KEY integer
    ---@field MOUSE_MOTION integer
    ---@field MOUSE_BUTTON integer
    ---@field GAMEPAD_BUTTON integer
    InputEventType = lysa.InputEventType,

    ---@class lysa.InputEventKey
    ---@field key integer
    ---@field pressed boolean
    ---@field repeat integer
    ---@field modifiers integer
    InputEventKey = lysa.InputEventKey,

    ---@class lysa.InputEvent
    ---@field type integer
    InputEvent = lysa.InputEvent,

    ---@class lysa.Event
    ---@field id integer
    ---@field type any
    Event = lysa.Event,

    ---@class lysa.EventManager
    ---@field push fun(self:lysa.EventManager, e:lysa.Event):nil
    ---@field subscribe fun(self:lysa.EventManager, type:any, id:any, cb:function):nil
    ---@field subscribe fun(self:lysa.EventManager, type:any, cb:function):nil
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
    ---@field READY any
    ---@field CLOSING any
    ---@field RESIZED any
    ---@field INPUT any
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
    -- Images / Textures / Samplers
    ------------------------------------------------------------------------

    ---@class lysa.Samplers
    ---@field NEAREST_NEAREST_CLAMP_TO_BORDER integer
    ---@field LINEAR_LINEAR_CLAMP_TO_EDGE integer
    ---@field LINEAR_LINEAR_CLAMP_TO_EDGE_LOD_CLAMP_NONE integer
    ---@field LINEAR_LINEAR_REPEAT integer
    ---@field NEAREST_NEAREST_REPEAT integer
    Samplers = lysa.Samplers,

    ---@class lysa.Image
    ---@field id integer
    ---@field width integer         @read-only (getter)
    ---@field height integer        @read-only (getter)
    ---@field size integer          @read-only (getter)
    ---@field name string           @read-only (getter)
    ---@field index integer         @read-only (getter)
    ---@field image any             @lightuserdata or backend handle
    Image = lysa.Image,

    ---@class lysa.ImageManager
    ---@field load fun(self:lysa.ImageManager, path:string):lysa.Image
    ---@field save fun(self:lysa.ImageManager, img:lysa.Image, path:string):nil
    ---@field blank_image lysa.Image
    ---@field blank_cube_map lysa.Image
    ---@field images fun(self:lysa.ImageManager):table<integer, lysa.Image>
    ---@field get fun(self:lysa.ImageManager, id:integer):lysa.Image
    ---@field destroy fun(self:lysa.ImageManager, id:integer):nil
    ImageManager = lysa.ImageManager,

    ---@class lysa.Texture
    ---@field id integer
    ---@field width integer         @read-only (getter)
    ---@field height integer        @read-only (getter)
    ---@field size integer          @read-only (getter)
    ---@field name string           @read-only (getter)
    Texture = lysa.Texture,

    ---@class lysa.ImageTexture
    ---@field id integer
    ---@field width integer         @read-only (getter)
    ---@field height integer        @read-only (getter)
    ---@field image lysa.Image      @read-only (getter)
    ---@field sampler_index integer @read-only (getter)
    ImageTexture = lysa.ImageTexture,

    ---@class lysa.ImageTextureManager
    ---@field create fun(self:lysa.ImageTextureManager, image:lysa.Image, sampler:integer):lysa.ImageTexture
    ---@field get fun(self:lysa.ImageTextureManager, id:integer):lysa.ImageTexture
    ---@field destroy fun(self:lysa.ImageTextureManager, id:integer):nil
    ImageTextureManager = lysa.ImageTextureManager,

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
    -- Materials
    ------------------------------------------------------------------------

    ---@class lysa.Transparency
    ---@field DISABLED integer
    ---@field ALPHA integer
    Transparency = lysa.Transparency,

    ---@class lysa.MaterialType
    ---@field STANDARD integer
    ---@field SHADER integer
    MaterialType = lysa.MaterialType,

    ---@class lysa.Material
    ---@field id integer
    ---@field cull_mode vireo.CullMode
    ---@field transparency integer   @lysa.Transparency
    ---@field alpha_scissor number
    ---@field index integer          @read-only (getter)
    ---@field type integer           @lysa.MaterialType, read-only
    Material = lysa.Material,

    ---@class lysa.TextureInfo
    ---@field texture lysa.ImageTexture|nil
    ---@field transform any          @float3x3
    TextureInfo = lysa.TextureInfo,

    ---@class lysa.StandardMaterial: lysa.Material
    ---@field albedo_color lysa.float4
    ---@field diffuse_texture lysa.TextureInfo|nil
    ---@field normal_texture lysa.TextureInfo|nil
    ---@field metallic_factor number
    ---@field metallic_texture lysa.TextureInfo|nil
    ---@field roughness_factor number
    ---@field roughness_texture lysa.TextureInfo|nil
    ---@field emissive_texture lysa.TextureInfo|nil
    ---@field emissive_factor lysa.float3
    ---@field emissive_strength number
    ---@field normal_scale number
    StandardMaterial = lysa.StandardMaterial,

    ---@class lysa.ShaderMaterial: lysa.Material
    ---@field frag_file_name string  @read-only (getter)
    ---@field vert_file_name string  @read-only (getter)
    ---@field set_parameter fun(self:lysa.ShaderMaterial, index:integer, value:lysa.float4):nil
    ---@field get_parameter fun(self:lysa.ShaderMaterial, index:integer):lysa.float4
    ShaderMaterial = lysa.ShaderMaterial,

    ---@class lysa.MaterialManager
    ---@field create_standard fun(self:lysa.MaterialManager):lysa.StandardMaterial
    ---@field create_shared fun(self:lysa.MaterialManager, frag:string, vert:string):lysa.ShaderMaterial
    ---@field get fun(self:lysa.MaterialManager, id:integer):lysa.Material
    MaterialManager = lysa.MaterialManager,

    ------------------------------------------------------------------------
    -- Geometry / Meshes / Scenes
    ------------------------------------------------------------------------

    ---@class lysa.AABB
    ---@field min lysa.float3
    ---@field max lysa.float3
    AABB = lysa.AABB,

    ---@class lysa.Vertex
    ---@field position lysa.float3
    ---@field normal lysa.float3
    ---@field uv lysa.float2
    ---@field tangent lysa.float4
    Vertex = lysa.Vertex,

    ---@class lysa.MeshSurface
    ---@field firstIndex integer
    ---@field indexCount integer
    ---@field material any           @lysa.Material or resource id
    MeshSurface = lysa.MeshSurface,

    ---@class lysa.Mesh
    ---@field id integer
    ---@field get_surface_material fun(self:lysa.Mesh, surfaceIndex:integer):lysa.Material|nil
    ---@field set_surface_material fun(self:lysa.Mesh, surfaceIndex:integer, mat:integer):nil
    ---@field aabb fun(self:lysa.Mesh):lysa.AABB
    Mesh = lysa.Mesh,

    ---@class lysa.MeshManager
    ---@field create fun(self:lysa.MeshManager):lysa.Mesh
    ---@field create fun(self:lysa.MeshManager, vertices:any, indices:any, surfaces:any):lysa.Mesh
    ---@field get fun(self:lysa.MeshManager, id:integer):lysa.Mesh
    MeshManager = lysa.MeshManager,

    ---@class lysa.Scene
    ---@field id integer
    ---@field ambientLight lysa.float3
    Scene = lysa.Scene,

    ---@class lysa.SceneManager
    ---@field create fun(self:lysa.SceneManager):lysa.Scene
    ---@field get fun(self:lysa.SceneManager, id:integer):lysa.Scene
    SceneManager = lysa.SceneManager,

    ------------------------------------------------------------------------
    ---@class lysa.ResourcesRegistry
    ---@field get fun(self:lysa.ResourcesRegistry, id:integer):any
    ---@field render_target_manager lysa.RenderTargetManager
    ---@field viewport_manager lysa.ViewportManager
    ---@field rendering_window_manager lysa.RenderingWindowManager
    ---@field image_manager lysa.ImageManager
    ---@field image_texture_manager lysa.ImageTextureManager
    ---@field material_manager lysa.MaterialManager
    ---@field mesh_manager lysa.MeshManager
    ---@field scene_manager lysa.sceneManager
    ResourcesRegistry = lysa.ResourcesRegistry,

    ---@class MainLoopEvent
    ---@field PROCESS any
    ---@field PHYSICS_PROCESS any
    ---@field QUIT any
    MainLoopEvent = lysa.MainLoopEvent,

    ---@class lysa.Context
    ---@field exit boolean
    ---@field vireo vireo.Vireo
    ---@field fs lysa.VirtualFS
    ---@field events lysa.EventManager
    ---@field world ecs
    ---@field res lysa.ResourcesRegistry
    ---@field graphic_queue vireo.SubmitQueue
    Context = lysa.Context,

    ---@class lysa.Context
    ctx = lysa.ctx,
}
