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
      "background": @appearance.panelBackground
      "position": "absolute"
      "left": "0px"
      "width": "46px"
      "height": "100%"
      "z-index": "1029"
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
