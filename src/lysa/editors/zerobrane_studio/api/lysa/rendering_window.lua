-- ZeroBrane Studio API autocomplete for Lysa RenderingWindow classes
return {
  lysa = {
    type = 'lib',
    description = 'Lysa Engine Lua API: RenderingWindow facilities.',
    childs = {

      -- Enum: RenderingWindowMode
      RenderingWindowMode = {
        type = 'table',
        description = 'Rendering window display modes (enum).',
        childs = {
          WINDOWED = { type = 'number', description = 'Window with border and title; can be minimized (0).'},
          WINDOWED_MAXIMIZED = { type = 'number', description = 'Maximized window with border and title (1).'},
          WINDOWED_FULLSCREEN = { type = 'number', description = 'Borderless maximized window (2).'},
          FULLSCREEN = { type = 'number', description = 'Exclusive full-screen; changes monitor resolution (3).'},
        }
      },

      -- Event constants: RenderingWindowEvent
      RenderingWindowEvent = {
        type = 'table',
        description = 'Rendering window event identifiers (string constants).',
        childs = {
          READY = { type = 'string', description = 'Emitted when a rendering window is ready.' },
          CLOSING = { type = 'string', description = 'Emitted when a rendering window is closing.' },
          RESIZED = { type = 'string', description = 'Emitted when a rendering window has been resized.' },
        }
      },

      -- Data struct: RenderingWindowConfiguration
      RenderingWindowConfiguration = {
        type = 'class',
        description = 'Configuration used when creating a rendering window.',
        -- Fields (can be set via table or constructor if provided by binding).
        childs = {
          title = { type = 'string', description = 'Window title bar. Default: "Lysa Window".' },
          mode = { type = 'table', description = 'Display mode. One of lysa.RenderingWindowMode values.' },
          x = { type = 'number', description = 'Startup X position in pixels (top-left). -1 for default.' },
          y = { type = 'number', description = 'Startup Y position in pixels (top-left). -1 for default.' },
          width = { type = 'number', description = 'Window width in pixels. Default: 1280.' },
          height = { type = 'number', description = 'Window height in pixels. Default: 720.' },
          monitor = { type = 'number', description = 'Monitor index to display the window on. Default: 0.' },
        }
      },

      -- Data struct: RenderingWindow
      RenderingWindow = {
        type = 'class',
        description = 'Operating system window serving as a rendering surface.',
        childs = {
          id = { type = 'number', description = 'Unique window identifier.' },
          x = { type = 'number', description = 'Top-left X position in pixels.' },
          y = { type = 'number', description = 'Top-left Y position in pixels.' },
          width = { type = 'number', description = 'Current width in pixels.' },
          height = { type = 'number', description = 'Current height in pixels.' },
          stopped = { type = 'boolean', description = 'True when the window has been requested to close.' },
          platformHandle = { type = 'lightuserdata', description = 'Opaque OS window handle used for presentation.' },
          locator = { type = 'userdata', description = 'Resource locator object (engine-internal).' },
        }
      },

      -- Manager: RenderingWindowManager
      RenderingWindowManager = {
        type = 'class',
        description = 'Manager for creating and controlling RenderingWindow resources.',
        args = '(ctx[, capacity:number])',
        returns = 'RenderingWindowManager',
        childs = {
          create = {
            type = 'method',
            description = 'Create a new rendering window resource.',
            args = '(configuration:lysa.RenderingWindowConfiguration)',
            returns = 'id:number',
          },
          closing = {
            type = 'method',
            description = 'Notify the manager that a window is closing (dispatches CLOSING event).',
            args = '(id:number)'
          },
          resized = {
            type = 'method',
            description = 'Notify the manager that a window has been resized (dispatches RESIZED event).',
            args = '(id:number)'
          },
          show = {
            type = 'method',
            description = 'Make a previously created window visible on screen.',
            args = '(id:number)'
          },
          get = {
            type = 'method',
            description = 'Retrieve a mutable reference to the resource with the given ID.',
            args = '(id:number)',
            returns = 'lysa.RenderingWindow'
          }
        }
      },

    }
  }
}
