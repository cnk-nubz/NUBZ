root = exports ? this
root.View = class View
  constructor: ->
    @_views = {}
    @_handlers = {}

  on: (name, handler) =>
    if not @_handlers[name]?
      @_handlers[name] = []
    @_handlers[name].push handler
    @

  off: (name) =>
    @_handlers[name] = []
    @

  fireEvents: (name, args...) =>
    h(args...) for h in @_handlers[name] if @_handlers[name]?
    @
