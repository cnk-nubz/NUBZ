root = exports ? this
root.JustMapPage = class JustMapPage extends root.View
  constructor: (@_containerId) ->
    super
    @canvas = new Canvas("#{@_containerId}-a")
    @_init()
    @addView("map", @canvas)


  _init: =>
    leftPanelStyle = {
      "background": "rgba(255, 255, 255, 1)"
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

    @select(@_containerId)
      .append "div"
      .attr(
        id: "left-panel"
      )
      .style(leftPanelStyle)
      .html "&nbsp;"

    @select("#{@_containerId}-a")
      .style(canvasStyle)
    @

  refresh: =>
    @canvas.refresh()
    @
