root = exports ? this
root.MutableCanvas = class MutableCanvas extends root.Canvas
  # constructor :: (String, CanvasData) -> Context
  constructor: (containerId, mapData) ->
    super
    @_isResizingAllowed = false


  # _exhibitOptions :: [JsObject] -> JsObject
  _exhibitOptions: (options...) -> jQuery.extend(super, { draggable: true })


  # _prepareExhibit :: L.Rectangle -> Exhibit
  _prepareExhibit: (exhibit) =>
    exhibit = super exhibit
    exhibit.editing.enable() if @_isResizingAllowed
    exhibit.on('editstart', @_onEditStart)
    exhibit.on('edit', @_onEditEnd)
    exhibit.on('dragstart', @_onDragStart)
    exhibit.on('dragend', @_onDragEnd)
    exhibit


  # _updateState :: Int -> undefined
  _updateState: (floor) ->
    super
    @_updateExhibitResizing(floor)
    return


  # _onEditStart :: Event -> undefined
  _onEditStart: (e) ->
    exhibit = e.target
    exhibit.hideLabel()
    exhibit.bringToFront()
    return


  # _onEditEnd :: Event -> undefined
  _onEditEnd: (e) =>
    exhibit = e.target
    @_fixExhibitPosition(exhibit)
    exhibit.editing._repositionCornerMarkers()
    @_updateExhibitPosition(exhibit)
    exhibit.showLabel() if @_areLabelsVisible
    return


  # _onDragStart :: Event -> undefined
  _onDragStart: (e) ->
    exhibit = e.target
    exhibit.hideLabel()
    exhibit.editing.disable()
    exhibit.bringToFront()
    return


  # _onDragEnd :: Event -> undefined
  _onDragEnd: (e) =>
    if not e.target._dragMoved
      return
    exhibit = e.target
    @_updateExhibitPosition(exhibit)
    exhibit.showLabel() if @_areLabelsVisible
    exhibit.editing.enable() if @_isResizingAllowed
    return


  # _updateExhibitPosition :: L.Rectangle -> undefined
  _updateExhibitPosition: (exhibit) =>
    geoPoints = @_fixExhibitPosition(exhibit)
    exhibitId = exhibit.options.exhibitId
    exhibitFloor = @_mapData.exhibits[exhibitId].mapFrame.floor
    scaledPoints = (@_map.project(p, @_mapData.maxZoom[exhibitFloor]) for p in geoPoints)
    [topLeft, bottomRight] = [@_getTopLeft(scaledPoints), @_getBottomRight(scaledPoints)]
    @_changeExhibitPositionRequest(exhibit.options.exhibitId, topLeft, bottomRight)
    return


  # _fixExhibitPosition :: L.Rectangle -> [L.LatLng]
  _fixExhibitPosition: (exhibit) =>
    exhibitId = exhibit.options.exhibitId
    exhibitFloor = @_mapData.exhibits[exhibitId].mapFrame.floor
    maxX = @_map.project(@_mapBounds[exhibitFloor].getNorthEast()).x
    maxY = @_map.project(@_mapBounds[exhibitFloor].getSouthWest()).y

    latLng = exhibit.getLatLngs()[0]
    exhibitPoints = (@_map.project(ll) for ll in latLng)
    #update points in edge cases
    [topLeft, bottomRight] = [@_getTopLeft(exhibitPoints), @_getBottomRight(exhibitPoints)]
    [dx, dy] = @_getExhibitProtrusion(topLeft, bottomRight, maxX, maxY)
    exhibitPoints = (new L.Point(Math.min(maxX, p.x + dx), Math.min(maxY, p.y + dy)) for p in exhibitPoints)
    newGeoPoints = (@_map.unproject(p) for p in exhibitPoints)
    exhibit.setBounds(newGeoPoints)
    return newGeoPoints


  # _getExhibitProtrusion :: (L.Point, L.Point, Int, Int) -> [Int]
  _getExhibitProtrusion: (topLeft, bottomRight, maxX, maxY) ->
    if topLeft.x < 0
      dx = -topLeft.x
    else if bottomRight.x > maxX
      dx = maxX - bottomRight.x
    if topLeft.y < 0
      dy = -topLeft.y
    else if bottomRight.y > maxY
      dy = maxY - bottomRight.y
    [dx ? 0, dy ? 0]


  # _changeExhibitPositionRequest :: (Int, L.Point, L.Point) -> undefined
  _changeExhibitPositionRequest: (exhibitId, topLeft, bottomRight) =>
    exhibitFrame =
      exhibitId: exhibitId
      frame:
        size:
          width: bottomRight.x - topLeft.x
          height: bottomRight.y - topLeft.y
        x: topLeft.x
        y: topLeft.y
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      url: '/updateExhibitPosition/'
      data: (jsonData: JSON.stringify(exhibitFrame))
      context: this
      success: => @_changeExhibitPositionSuccess(exhibitFrame)
      error: @_displayError
    )
    return


  # _changeExhibitPositionSuccess :: ExhibitFrame -> undefined
  _changeExhibitPositionSuccess: (data) =>
    exhibitId = data.exhibitId
    @_mapData.exhibits[data.exhibitId].mapFrame.frame = data.frame
    return


  # _getTopLeft :: [L.Point] -> L.Point
  _getTopLeft: (arr) ->
    top = arr[0]
    (top = p if p.x <= top.x and p.y <= top.y) for p in arr[1..]
    top


  # _getBottomRight :: [L.Point] -> L.Point
  _getBottomRight: (arr) ->
    bot = arr[0]
    (bot = p if p.x >= bot.x and p.y >= bot.y) for p in arr[1..]
    bot


  # setExhibitResizing :: (Boolean, Int) -> Context
  setExhibitResizing: (@_isResizingAllowed, floor) =>
    @_updateExhibitResizing(floor)
    @


  # _updateExhibitResizing :: Int -> Context
  _updateExhibitResizing: (floor) =>
    @_exhibits[floor].eachLayer((layer) =>
      if @_isResizingAllowed
        layer.editing.enable()
      else
        layer.editing.disable()
    )
    @

  # _displayError :: jqXHR -> undefined
  _displayError: (obj) ->
    BootstrapDialog.show(
      message: obj.responseText
      title: obj.statusText
      type: BootstrapDialog.TYPE_DANGER
    )
    return
