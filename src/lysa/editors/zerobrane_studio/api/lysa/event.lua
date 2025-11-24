-- ZeroBrane Studio API autocomplete for Lysa Event system
return {
  lysa = {
    type = 'lib',
    description = 'Lysa Engine Lua API: Event and EventManager.',
    childs = {

      -- Message: Event
      Event = {
        type = 'class',
        description = 'Generic event message dispatched through the EventManager.',
        childs = {
          id = { type = 'number', description = 'Target object/resource unique identifier.' },
          type = { type = 'string', description = 'Event type name.' },
        }
      },

      -- Dispatcher: EventManager
      EventManager = {
        type = 'class',
        description = 'Simple event manager supporting C++ and Lua handlers.',
        childs = {
          push = {
            type = 'method',
            description = 'Enqueue an event to be delivered on next processing.',
            args = '(event:lysa.Event)'
          },
          subscribe = {
            type = 'method',
            description = 'Subscribe a Lua handler to a given event type and target id. The handler is called with (event).',
            args = '(type:string, id:number, handler:function)'
          },
        }
      },

    }
  }
}
