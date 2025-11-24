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
          resources_locator = {
            type = 'userdata',
            description = 'Resource resolution and access facility (lysa.ResourcesLocator).'
          },
        }
      },

      -- Global property: ctx
      ctx = {
        type = 'lysa.Context',
        description = 'Global application context instance exposed by the engine.',
        childs = {
          exit = { type = 'boolean', description = 'Quit flag controlling the main loop termination.' },
          event_manager = {
            type = 'userdata',
            description = 'Central event dispatcher for the application (lysa.EventManager).'
          },
          resources_locator = {
            type = 'userdata',
            description = 'Resource resolution and access facility (lysa.ResourcesLocator).'
          },
        }
      },

    }
  }
}
