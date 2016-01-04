root = exports ? this
root.ControllerEmitter = class ControllerEmitter extends root.Controller
  constructor: ->
    super
    @_handler = {}

  on: (name, handler) =>
    if not @_handler[name]?
      @_handler[name] = []
    @_handler[name].push handler
    @

  off: (name) =>
    @_handler[name] = []
    @
