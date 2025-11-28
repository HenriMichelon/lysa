-- ZeroBrane Studio API autocomplete for Lysa Context
return {
  lysa = {
    type = 'lib',
    description = 'Lysa Engine Lua API: application Context and accessors.',
    childs = {

      -- Class: Context
      Context = {
        type = 'class',
        description = 'Lysa instance-wide runtime context accessible from Lua.',
        childs = {
          exit = { type = 'boolean', description = 'Quit flag controlling the main loop termination.' },
          event_manager = {
            type = 'userdata',
            description = 'Central event dispatcher for the application (lysa.EventManager).'
          },
          resourcesLocator = {
            type = 'userdata',
            description = 'Resource resolution and access facility (lysa.ResourcesLocator).'
          },
          vireo = {
            type = 'userdata',
            description = 'Backend object owning the device/instance and factory for GPU resources.'
          },
          graphicQueue = {
            type = 'userdata',
            description = 'Submit queue used for graphics/rendering work.'
          },
        }
      },

      -- Global property: ctx
      ctx = {
        type = 'lysa.Context',
        description = 'Global application context instance exposed by the engine.',
        childs = {
          exit = { type = 'boolean', description = 'Quit flag controlling the main loop termination.' },
          eventManager = {
            type = 'userdata',
            description = 'Central event dispatcher for the application (lysa.EventManager).'
          },
          resourcesLocator = {
            type = 'userdata',
            description = 'Resource resolution and access facility (lysa.ResourcesLocator).'
          },
          vireo = {
            type = 'userdata',
            description = 'Backend object owning the device/instance and factory for GPU resources.'
          },
          graphicQueue = {
            type = 'userdata',
            description = 'Submit queue used for graphics/rendering work.'
          },
        }
      },

    }
  }
}
