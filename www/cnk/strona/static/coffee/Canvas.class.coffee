root = exports ? this
root.Canvas = class Canvas extends root.View
  # constructor :: (String, MapData) -> Context
  constructor: (@_containerMap, @_mapData) ->
    super()
    @_minZoom = 1
    if not @_mapData.floorTilesInfo?
      @_maxZoom = [1]
    else
      @_maxZoom = @_mapData.floorTilesInfo.map(
        (x) =>
          if x?
            Object.keys(x).length
          else
            @_minZoom
      )
    @_mapBounds = []
    @_exhibits = (new L.LayerGroup for i in @_mapData.availableFloors)
    @_floorLayer = (new L.LayerGroup for i in @_mapData.availableFloors)
    @_dataLoaded = (false for i in @_mapData.availableFloors)
    @_map = L.map(@_containerMap[1..], {
      minZoom: @_minZoom
      zoom: @_minZoom
      center: [0, 0]
      crs: L.CRS.Simple
      autoPan: false
      zoomControl: false
    })
    @_areLabelsVisible = true
    @_setHandlers()


  # _setHandlers :: () -> undefined
  _setHandlers: =>
    @_map.on('zoomend', =>
      disableMinus = @_map.getZoom() is @_minZoom or @_minZoom is @_maxZoom[@_mapData.activeFloor]
      disablePlus = @_map.getZoom() >= @_maxZoom[@_mapData.activeFloor]
      @fireEvents('zoomend', disableMinus, disablePlus)
    )
    return


  # _loadData :: Int -> undefined
  _loadData: (floor) =>
    tileInfo = @_mapData.floorTilesInfo[floor]
    if tileInfo.length is 0
      @_dataLoaded[floor] = true
      return
    @_addMapBounds(floor, [0, tileInfo[-1..][0].scaledHeight], [tileInfo[-1..][0].scaledWidth, 0])
    @_addFloorLayer(floor)
    @addExhibits((e for id, e of @_mapData.exhibits when e.frame?.mapLevel is floor))
    @_dataLoaded[floor] = true
    return


  # _addMapBounds :: (Int, LatLng, LatLng) -> Context
  _addMapBounds: (floor, northEast, southWest) =>
    @_mapBounds[floor] = new L.LatLngBounds(@_map.unproject(northEast, @_maxZoom[floor]),
                                            @_map.unproject(southWest, @_maxZoom[floor]))
    @


  # _addFloorLayer :: Int -> Context
  _addFloorLayer: (floor) =>
    url = "#{@_getFloorUrl(floor)}?t=#{Math.floor(Math.random() * 1021)}"
    tileInfo = @_mapData.floorTilesInfo[floor]
    for j in [0...tileInfo.length]
      zoomLayer = L.tileLayer(url.replace('{z}', "#{j + @_minZoom}"), {
        minZoom: j + @_minZoom
        maxZoom: j + @_minZoom
        tileSize: tileInfo[j].tileWidth
        continuousWorld: true
        crs: L.CRS.Simple
        bounds: @_mapBounds[floor]
      })
      @_floorLayer[floor].addLayer zoomLayer
    @


  # getFloorUrl :: Int -> String
  _getFloorUrl: (floor) =>
    @_mapData.floorUrl.replace("{f}", "#{floor}")


  # addExhibits :: [Exhibit] -> Context
  addExhibits: (exhibits) =>
    for e in exhibits
      floor = e.frame.mapLevel
      X = e.frame.x
      Y = e.frame.y
      polygonBounds = new L.LatLngBounds(
        @_map.unproject([X, Y], @_maxZoom[floor]),
        @_map.unproject([X + e.frame.width, Y + e.frame.height], @_maxZoom[floor]),
      )
      options =
          fillColor: e.colorHex
          fillOpacity: 0.7
          weight: 1
          strokeColor: '#B4AFD1'
          strokeOpacity: 1
      r = L.rectangle(polygonBounds, @_exhibitOptions(options, { id: e.id }))
      r.bindLabel(e.name, { direction: 'auto' })
      r = @_prepareExhibit(r)
      @_exhibits[floor].addLayer(r)
    @


  # removeExhibit :: Int -> Context
  removeExhibit: (exhibitId) =>
    exhibitFrame = @_mapData.exhibits[exhibitId].frame
    if not exhibitFrame?
      return
    @_exhibits[exhibitFrame.mapLevel].eachLayer((layer) =>
      if parseInt(layer.options.id) is exhibitId
        @_exhibits[exhibitFrame.mapLevel].removeLayer(layer)
    )
    @


  # _exhibitOptions :: [JsObject] -> JsObject
  _exhibitOptions: (options...) -> jQuery.extend(options...)


  # _prepareExhibit :: L.Rectangle -> L.Rectangle
  _prepareExhibit: (exh) -> exh


  # setFloorLayer :: (Int, Int) -> Context
  setFloorLayer: (newFloor, oldFloor) =>
    if not @_dataLoaded[newFloor]
      @_loadData newFloor
    @_map.removeLayer(@_exhibits[oldFloor])
    @_map.removeLayer(@_floorLayer[oldFloor])
    @_map.addLayer(@_floorLayer[newFloor])
    @_map.addLayer(@_exhibits[newFloor])
    @_map.setView([0, 0], @_minZoom, animate: false)
    @_map.fireEvent('zoomend')
    @_map.setMaxBounds(@_mapBounds[newFloor])
    @_map.invalidateSize()
    @updateLabelsVisibility()
    @


  # getVisibleFrame :: Int -> (L.Point, Int, Int)
  getVisibleFrame: (activeFloor) =>
    bounds = @_map.getBounds()
    maxZoomTileInfo = @_mapData.floorTilesInfo[activeFloor][-1..][0]
    maxX = maxZoomTileInfo?.scaledWidth ? 0
    maxY = maxZoomTileInfo?.scaledHeight ? 0
    maxZoom = @_maxZoom[activeFloor]
    castedPixelBounds = [
        @_map.project(bounds.getNorthWest(), maxZoom)
        @_map.project(bounds.getSouthEast(), maxZoom)
    ]
    min = castedPixelBounds[0]
    max = castedPixelBounds[1]
    topLeft = new L.Point(Math.min(maxX, Math.max(0, min.x)), Math.min(maxY, Math.max(0, min.y)))
    bottomRight = new L.Point(Math.min(maxX, Math.max(0, max.x)), Math.min(maxY, Math.max(0, max.y)))
    [topLeft, width = bottomRight.x - topLeft.x, height = bottomRight.y - topLeft.y]


  # flyToExhibit :: Int -> Context
  flyToExhibit: (exhibitsId) =>
    floor = @_mapData.activeFloor
    exhibit = @_mapData.exhibits[exhibitsId]
    frame = exhibit.frame
    bounds = new L.LatLngBounds(
      @_map.unproject([frame.x, frame.y], @_maxZoom[floor]),
      @_map.unproject([frame.x + frame.width, frame.y + frame.height], @_maxZoom[floor]),
    )
    if floor isnt frame.mapLevel
      if not @_dataLoaded[frame.mapLevel]
        @_loadData frame.mapLevel
      @_floorLayer[frame.mapLevel].getLayers()[0].once("load", =>
        @_map.flyToBounds(bounds, animate: false)
        @_map.fireEvent('zoomend')
      )
      @setFloorLayer(frame.mapLevel, floor)
    else
      @_map.flyToBounds(bounds, animate: false)
      @_map.fireEvent('zoomend')
    @


  # updateLabelsVisibility :: Boolean -> Context
  updateLabelsVisibility: (areVisible = @_areLabelsVisible) =>
    @_areLabelsVisible = areVisible
    @_exhibits[@_mapData.activeFloor].eachLayer((layer) ->
      if areVisible
        layer.showLabel()
      else
        layer.hideLabel()
    )
    @


  # zoomIn :: () -> Context
  zoomIn: =>
    @_map.zoomIn()
    @


  # zoomOut :: () -> Context
  zoomOut: =>
    @_map.zoomOut()
    @
