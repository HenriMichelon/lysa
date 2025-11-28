-- ZeroBrane Studio API autocomplete for Lysa RenderingWindow classes
return {
  lysa = {
    type = 'lib',
    description = 'Lysa Engine Lua API: RenderTarget facilities.',
    childs = {

      RenderTargetConfiguration = {
        type = 'class',
        description = 'Configuration used when creating a render target.',
        -- Fields (can be set via table or constructor if provided by binding).
        childs = {
          renderingWindowHandle = { type = 'lightuserdata', description = 'Set this field if you want to render in a window.' },
          swapChainFormat = { type = 'table', description = 'Postprocessing & swap chain image format.' },
          presentMode = { type = 'table', description = 'Presentation mode.' },
          framesInFlight = { type = 'number', description = ' Number of simultaneous frames during rendering' },
        }
      },

      RenderTarget = {
        type = "class",
        description = "Render target wrapper used by the engine.",
        childs = {
          id = {
            type = "number",
            description = "Unique ID (unique_id).",
          },
          paused = {
            type = "boolean",
            description = "If true, rendering on this target is paused.",
          },
          swapChain = {
            type = "function",
            description = "Swap chain presenting the render target (shared_ptr<vireo::SwapChain>).",
          },
        },

        RenderTargetManager = {
          type = 'class',
          description = 'Manager for creating and controlling RenderTarget resources.',
          args = '(ctx[, capacity:number])',
          returns = 'RenderTargetManager',
          childs = {
            create = {
              type = 'method',
              description = 'Create a render target.',
              args = '(configuration:lysa.RenderTargetConfiguration)',
              returns = 'id:number',
            },
            destroy = {
              type = 'method',
              description = 'Destroy a render target.',
              args = '(render_target:lysa.RenderTarget)',
            },
            get = {
              type = 'method',
              description = 'Retrieve a mutable reference to the resource with the given ID.',
              args = '(id:number)',
              returns = 'lysa.RenderTarget'
            }
          }
        },
      }
    }
  }
}
