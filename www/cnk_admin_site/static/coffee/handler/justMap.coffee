root = exports ? this
root.cachedData = {}
class Handlers
  # constructor :: MapData -> Context
  constructor: (@_mapData) ->
    @_mapData.minZoom = @_mapData.floorTilesInfo.map( -> 1)
    @_mapData.maxZoom = @_mapData.floorTilesInfo.map((x) -> Math.max(Object.keys(x).length, 1))
    @_DOM =
      plusZoom: "#zoomControls button:first-child"
      minusZoom: "#zoomControls button:last-child"
      floorButton: "#changeFloor button"
      labels: "#showLabels button"

    @canvas = new root.Canvas('#map', @_mapData)
    @_setButtonHandlers()
    @_setEvents()
    jQuery("#{@_DOM.labels}, #{@_getNthFloor(@_mapData.activeFloor)}").addClass "active"
    @canvas.setFloorLayer 0, 0


  # _getNthFloor :: Int -> String
  _getNthFloor: (n) ->
    "#{@_DOM.floorButton}:eq(#{n})"


  # _setButtonHandlers :: () -> undefined
  _setButtonHandlers: =>
    jQuery(@_DOM.minusZoom).click(canvas: @canvas, @zoomOutHandler)
    jQuery(@_DOM.plusZoom).click(canvas: @canvas, @zoomInHandler)
    floorButtons = @_mapData.availableFloors.map((n) => jQuery(@_getNthFloor(n)))
    for button, idx in floorButtons
      data =
        instance: this
        floorButtons: floorButtons
      button.click(data, @changeFloorHandler(idx))
    jQuery(@_DOM.labels).click(instance: this, @showLabelsHandler)
    return


  # _setEvents :: () -> undefined
  _setEvents: =>
    @canvas.on("zoomChange", (activeZoom) =>
      jQuery(@_DOM.plusZoom)
        .prop("disabled", activeZoom is @_mapData.maxZoom[@_mapData.activeFloor])
      jQuery(@_DOM.minusZoom)
        .prop("disabled", activeZoom is @_mapData.minZoom[@_mapData.activeFloor])
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
  changeFloorHandler: (newFloor) ->
    (e) ->
      oldFloor = e.data.instance._mapData.activeFloor
      e.data.instance._mapData.activeFloor = newFloor
      jQuery(e.data.floorButtons[oldFloor]).removeClass("active")
      jQuery(this).blur().addClass("active")
      e.data.instance.canvas.setFloorLayer(newFloor, oldFloor)
      return


  # showLabelsHandler :: Event -> undefined
  showLabelsHandler: (e) ->
    instance = e.data.instance
    obj = jQuery(this)
    obj.blur()
    isActive = obj.hasClass("active")
    if isActive
      obj.removeClass("active")
    else
      obj.addClass("active")

    activeFloor = instance._mapData.activeFloor
    instance.canvas.setLabelsVisibility(not isActive, activeFloor)
    return


jQuery(document).ready( ->
  mapData =
    activeFloor: root.activeFloor
    floorUrl: root.floorUrl
    exhibitsList: root.exhibitsList
    exhibits: root.exhibits
    floorTilesInfo: root.floorTilesInfo
    availableFloors: root.availableFloors
  handlers = new Handlers(mapData)
)
