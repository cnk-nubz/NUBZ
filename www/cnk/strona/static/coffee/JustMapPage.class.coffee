root = exports ? this
root.JustMapPage = class JustMapPage extends root.View
  constructor: (@_containerId) ->
    super
    @appearance = new Appearance()
    @canvas = new Canvas("#{@_containerId}-a")
    @_init()
    @addView("map", @canvas)

  _init: =>
    @select(@_containerId)
      .append "div"
      .attr(
        id: "left-panel"
      )
      .html "&nbsp;"
    @_initCss()
    @

  _initCss: =>
    leftPanelStyle = {
      "background": @appearance.panel.background
      "position": "absolute"
      "top": @appearance.navbar.height
      "left": "0px"
      "bottom": "0px"
      "width": @appearance.panel.mapControls.width
      "z-index": @appearance.panel.zindex
      "border-right": @appearance.panel.border
    }
    canvasStyle = {
      "position": "relative"
      "margin-left": @appearance.panel.mapControls.width
      "height": "100%"
      "overflow": "visible"
    }
    @select(@_containerId).select("#left-panel")
      .style(leftPanelStyle)
    @select("#{@_containerId}-a")
      .style(canvasStyle)

    @
