root = exports ? this
root.Canvas = class Canvas extends root.View
  constructor: (@_containerMap) ->
    super
    @mapData = new MapDataHandler()
    @_minZoom = @mapData.minZoom
    @_maxZoom = @mapData.maxZoom
    @_mapBounds = [null, null]
    @_exhibits = [new L.LayerGroup(), new L.LayerGroup()]
    @_floorLayer = [new L.LayerGroup(), new L.LayerGroup()]
    @_map = L.map(@_containerMap[1..], {
      minZoom: @_minZoom
      zoom: @_minZoom
      center: [0, 0]
      crs: L.CRS.Simple
      autoPan: false
      zoomControl: false
    })
    @_areLabelsVisible = true
    @_init()
    return

  _init: =>
    #TODO
    #@_map.on('moveend', @_getCurrentView)
    #@_map.on('zoomend', @_getCurrentView)
    @_map.on('zoomend', =>
      disableMinus = @_map.getZoom() is @_minZoom
      disablePlus = @_map.getZoom() is @_maxZoom[@mapData.activeFloor]
      @fireEvents('zoomend', disableMinus, disablePlus)
    )
    actv = @mapData.activeFloor
    @loadData i for i in [1-actv..actv]
    @

  _getCurrentView: =>
    floor = @mapData.activeFloor
    @mapData.currentZoom[floor] = @_map.getZoom()
    @mapData.currentCenter[floor] = @_map.getBounds().getCenter()
    @

  loadData: (floor) =>
    tileInfo = @mapData.floorTilesInfo[floor]
    rand = Math.floor(Math.random() * 1024)
    newUrl = "#{@mapData.floorUrl[floor]}?t=#{rand}"
    @_floorLayer[floor].clearLayers()
    @_exhibits[floor].clearLayers()
    @_addMapBounds(floor, [0, tileInfo[-1..][0].scaledHeight], [tileInfo[-1..][0].scaledWidth, 0])
    @addFloorLayer(floor, tileInfo, newUrl)
    @addExhibits(floor, (id for id, _ of @mapData.exhibits))
    @setFloorLayer(floor)
    @

  _addMapBounds: (floor, northEast, southWest) =>
    @_mapBounds[floor] = new L.LatLngBounds(@_map.unproject(northEast, @_maxZoom[floor]),
                                            @_map.unproject(southWest, @_maxZoom[floor]))
    @

  addFloorLayer: (floor, tileInfo, url) =>
    len = tileInfo.length
    for j in [0...len]
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

  addExhibits: (floor, exhibitIdList) =>
    for id in exhibitIdList
      e = @mapData.exhibits[id]
      continue unless e.frame?.mapLevel is floor
      X = e.frame.x
      Y = e.frame.y
      polygonBounds = new L.LatLngBounds(
        @_map.unproject([X, Y], @_maxZoom[floor]),
        @_map.unproject([X + e.frame.width, Y + e.frame.height], @_maxZoom[floor]),
      )
      options =
          fillColor: @_rgb2hex(e.frame.colorR, e.frame.colorG, e.frame.colorB)
          fillOpacity: 0.7
          weight: 1
          strokeColor: 'darkblue'
          strokeOpacity: 1
      r = L.rectangle(polygonBounds, @_exhibitOptions(options, { id: id }))
      r.bindLabel(e.name, { direction: 'auto' })
      @_prepareExhibit(r)
      @_exhibits[floor].addLayer(r)
    @

  _rgb2hex: (colorR, colorG, colorB) ->
    hex = (x) -> "0#{parseInt(x).toString(16)}"[-2..]
    '#' + hex(colorR) + hex(colorG) + hex(colorB)

  _exhibitOptions: (options...) =>
    jQuery.extend(options...)

  _prepareExhibit: (exh) =>
    return

  setFloorLayer: (floor) =>
      @mapData.activeFloor = floor
      @updateState()
      @

  updateState: =>
      floor = @mapData.activeFloor
      @_map.removeLayer(@_exhibits[1 - floor])
      @_map.removeLayer(@_floorLayer[1 - floor])
      @_map.addLayer(@_floorLayer[floor])
      @_map.addLayer(@_exhibits[floor])
      @changeLabelsVisibility(@_areLabelsVisible)
      @_map.setMaxBounds @_mapBounds[floor]
      @_map.invalidateSize()
      @

  refresh: =>
    @loadData(@mapData.activeFloor)
    @

  getVisibleFrame: =>
    bounds = @_map.getBounds()
    maxZoomTileInfo = @mapData.floorTilesInfo[@mapData.activeFloor][-1..][0]
    maxX = maxZoomTileInfo.scaledWidth
    maxY = maxZoomTileInfo.scaledHeight
    maxZoom = @mapData.maxZoom[@mapData.activeFloor]
    castedPixelBounds = [
        @_map.project(bounds.getNorthWest(), maxZoom)
        @_map.project(bounds.getSouthEast(), maxZoom)
    ]
    min = castedPixelBounds[0]
    max = castedPixelBounds[1]
    topLeft = new L.Point(Math.min(maxX, Math.max(0, min.x)), Math.min(maxY, Math.max(0, min.y)))
    bottomRight = new L.Point(Math.min(maxX, Math.max(0, max.x)), Math.min(maxY, Math.max(0, max.y)))
    [topLeft, width = bottomRight.x - topLeft.x, height = bottomRight.y - topLeft.y]

  flyToExhibit: (exhibitsId) =>
    floor = @mapData.activeFloor
    exhibit = @mapData.exhibits[exhibitsId]
    frame = exhibit.frame
    bounds = new L.LatLngBounds(
      @_map.unproject([frame.x, frame.y], @_maxZoom[floor]),
      @_map.unproject([frame.x + frame.width, frame.y + frame.height], @_maxZoom[floor]),
    )
    if floor isnt frame.mapLevel
      @_floorLayer[frame.mapLevel].getLayers()[0].once("load", =>
        @_map.flyToBounds(bounds, animate: false)
      )
      @setFloorLayer(frame.mapLevel)
    else
      @_map.flyToBounds(bounds, animate: false)
    return

  changeLabelsVisibility: (areVisible) =>
    @_areLabelsVisible = areVisible
    @_exhibits[@mapData.activeFloor].eachLayer((layer) ->
      if areVisible
        layer.showLabel()
      else
        layer.hideLabel()
    )

  zoomIn: =>
    @_map.zoomIn()

  zoomOut: =>
    @_map.zoomOut()
