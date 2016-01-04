root = exports ? this
root.Controller = class Controller
  constructor: (@_containerId) ->
    @_view = {}

  css: (name, css) =>
    view = @getView name
    viewId = view?._containerId
    if css?
      jQuery(viewId).css(css)
      return @
    else
      return jQuery(viewId).css()

  addClass: (name, klass) =>
    view = @getView name
    viewId = view?._containerId
    jQuery(viewId).addClass(klass)

  removeClass: (name, klass) =>
    view = @getView name
    viewId = view?._containerId
    jQuery(viewId).removeClass(klass)
    @

  addView: (name, view) =>
    @_view[name] = view
    @

  materializeView: (name) =>
    view = @getView name
    #make div ready to append to container
    document.detached[name] =
      d3.select(document.createElement("div"))
        .attr(
          id: view._containerId[1..]
        )
    view.spawn document.detached[name]
    @

  destroyView: (name) =>
    view = @getView name
    viewId = view?._containerId
    if view?.destroy?
      view.destroy()
    d3.select(viewId).remove() if viewId?
    @

  getView: (name) =>
    if @_view[name]?
      return @_view[name]
    undefined #no view with this name

  refreshView: (name) =>
    @_view[name]?.refresh()
    @

  setActiveView: (name) =>
    @_activeView = name

  clearViews: =>
    @_view = {}
    @
