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
      "width": @appearance.panel.left.width
      "z-index": "1029"
      "border-right": "1px solid #080808"
    }
    canvasStyle = {
      "position": "relative"
      "margin-left": "46px"
      "height": "100%"
      "overflow": "visible"
    }
    @select(@_containerId).select("#left-panel")
      .style(leftPanelStyle)
    @select("#{@_containerId}-a")
      .style(canvasStyle)

    @
