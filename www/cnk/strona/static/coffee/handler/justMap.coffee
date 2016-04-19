root = exports ? this
class Handlers
  # constructor :: () -> Context
  constructor: ->
    @mapData = new MapDataHandler()
    @_DOM =
      plusZoom: "#zoomControls button:first-child"
      minusZoom: "#zoomControls button:last-child"
      floorButton: "#changeFloor button:first-child"
      labels: "#showLabels button"

    @canvas = new root.Canvas('#map')
    @_setButtonHandlers()
    @_setEvents()
    jQuery("#{@_DOM.labels}, #{@_getNthFloor(@mapData.activeFloor)}").addClass "active"
    @canvas.setFloorLayer 0

  # _getNthFloor :: Int -> String
  _getNthFloor: (n) ->
    "#{@_DOM.floorButton} #{
      if n is 0
        ""
      else
        "+ " + "button".repeat(n).match(/.{6}/g).join(" + ")
    }"


  # _setButtonHandlers :: () -> undefined
  _setButtonHandlers: =>
    jQuery(@_DOM.minusZoom).click(canvas: @canvas, @zoomOutHandler)
    jQuery(@_DOM.plusZoom).click(canvas: @canvas, @zoomInHandler)
    instance = this
    floorButtons = @mapData.avaibleFloors.map((n) => jQuery(@_getNthFloor(n)))
    for button, idx in floorButtons
      data =
        instance: this
        floorButtons: floorButtons
      button.click(data, @changeFloorHandler(idx))
    jQuery(@_DOM.labels).click(canvas: @canvas, @showLabelsHandler)
    return


  # _setEvents :: () -> undefined
  _setEvents: =>
    @canvas.on("zoomend", (disableMinus, disablePlus) =>
      jQuery(@_DOM.plusZoom).prop("disabled", disablePlus)
      jQuery(@_DOM.minusZoom).prop("disabled", disableMinus)
    )
    return


  # zoomOutHandler :: Event -> undefined
  zoomOutHandler: (e) ->
    jQuery(this).blur()
    e.data.canvas.zoomOut()
    return


  # zoomInHandler :: Event -> undefined
  zoomInHandler: (e) ->
    jQuery(this).blur()
    e.data.canvas.zoomIn()
    return


  # changeFloorHandler :: Int -> Event -> undefined
  changeFloorHandler: (floor) ->
    (e) ->
      activeFloor = e.data.instance.mapData.activeFloor
      jQuery(e.data.floorButtons[activeFloor]).removeClass("active")
      jQuery(this).blur().addClass("active")
      e.data.instance.canvas.setFloorLayer(floor)
      return


  # showLabelsHandler :: Event -> undefined
  showLabelsHandler: (e) ->
    obj = jQuery(this)
    obj.blur()
    isActive = obj.hasClass("active")
    if isActive
      obj.removeClass("active")
    else
      obj.addClass("active")
    e.data.canvas.updateLabelsVisibility(not isActive)
    return


jQuery(document).ready( ->
  handlers = new Handlers()
)
