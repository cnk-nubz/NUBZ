root = exports ? this
root.Controller = class Controller
  constructor: (@_containerId) ->
    @_views = {}
    @_handlers = {}
    if d3.select(@_containerId).empty()
      document.detached[@_containerId] =
        d3.select(document.createElement("div"))
          .attr(
            id: @_containerId[1..]
          )

  select: (id) =>
    dom = d3.select(id)
    if dom.empty()
      return document.detached[id]
    else
      return dom

  addView: (name, view) =>
    @_views[name] = view
    viewContainer = @select(@_views[name]._containerId)
    @select(@_containerId).append -> viewContainer.node()
    @

  destroyView: (name) =>
    d3.select(@getView(name)._containerId).remove()
    @

  getView: (name) =>
    return @_views[name] if @_views[name]?
    throw new Error "no view named #{name}"

  refresh: =>
    view.refresh() for name, view of @_views
    @

  on: (name, handler) =>
    if not @_handlers[name]?
      @_handlers[name] = []
    @_handlers[name].push handler
    @

  off: (name) =>
    @_handlers[name] = []
    @

  fireEvents: (name) =>
    h() for h in @_handlers[name] if @_handlers[name]?
    @
