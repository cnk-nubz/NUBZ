root = exports ? this
root.Canvas = class Canvas extends root.View
  constructor: (@_containerMap) ->
    super @_containerMap
    @mapData = new MapDataHandler()
    @appearance = new Appearance()
    @_minZoom = @mapData.minZoom
    @_maxZoom = @mapData.maxZoom
    @_mapBounds = [null, null]
    @_exhibits = [new L.LayerGroup(), new L.LayerGroup()]
    @_floorLayer = [new L.LayerGroup(), new L.LayerGroup()]
    @_map = L.map(@select(@_containerMap).node(), {
      minZoom: @_minZoom
      zoom: @_minZoom
      crs: L.CRS.Simple
    })
    @_initButtons()
    @_init()

  _initButtons: =>
    @_labelsButton = L.easyButton({
      states:[
          {
            stateName: 'setLabels'
            icon: 'fa-comment-o'
            title: 'Pokaż etykiety'
            onClick: (btn) =>
              jQuery btn.button
                .addClass "clicked"
              btn.state('removeLabels')
              @mapData.activeLabels = true
              @_exhibits[@mapData.activeFloor].eachLayer((layer) ->
                layer.showLabel()
              )
              return
          },
          {
            stateName: 'removeLabels'
            icon: 'fa-comment-o'
            title: 'Ukryj etykiety'
            onClick: (btn) =>
              jQuery btn.button
                .removeClass "clicked"
              btn.state('setLabels')
              @mapData.activeLabels = false
              @_exhibits[@mapData.activeFloor].eachLayer((layer) ->
                layer.hideLabel()
              )
              return
          }
      ]
    })
    activeLabelState = "#{if @mapData.activeLabels then "removeLabels" else "setLabels"}"
    activeLabelButton = @_labelsButton.state(activeLabelState).button
    jQuery(activeLabelButton).addClass("clicked") if @mapData.activeLabels
    @_labelsButton.state

    @_floorButton = [
      L.easyButton('<strong>0</strong>', @setFloorLayer 0, 'Piętro 0').addTo @_map
      L.easyButton('<strong>1</strong>', @setFloorLayer 1, 'Piętro 1').addTo @_map
    ]

    toAddButton = @select(@_floorButton[1].getContainer())
    mainButton = @select(@_floorButton[0].getContainer())
        .append -> toAddButton[0][0].firstChild
    @_labelsButton.addTo @_map
    return

  _init: =>
    #TODO
    #@_map.on('moveend', @_getCurrentView)
    #@_map.on('zoomend', @_getCurrentView)
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
    @addExhibits(floor, @mapData.exhibits)
    @setFloorLayer(floor)(@_floorButton[floor])
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

  addExhibits: (floor, exhibits) =>
    for idx, e of exhibits
      continue unless e.frame?.mapLevel is floor
      X = e.frame.x
      Y = e.frame.y
      polygonBounds = new L.LatLngBounds(
        @_map.unproject([X, Y], @_maxZoom[floor]),
        @_map.unproject([X + e.frame.width, Y + e.frame.height], @_maxZoom[floor]),
      )
      options =
        color: @appearance.exhibit.strokeColor
        fillColor: @appearance.exhibit.fillColor
        fillOpacity: @appearance.exhibit.fillOpacity
        weight: @appearance.exhibit.weight
        strokeColor: @appearance.exhibit.strokeColor
        strokeOpacity: @appearance.exhibit.strokeOpacity
      r = L.rectangle(polygonBounds, @_exhibitOptions(options, { id: idx }))
      r.bindLabel(e.name, { direction: 'auto' })
      @_prepareExhibit(r)
      @_exhibits[floor].addLayer(r)
    @

  _exhibitOptions: (options...) =>
    jQuery.extend(options...)

  _prepareExhibit: (exh) =>
    return

  setFloorLayer: (floor) =>
    (btn) =>
      jQuery btn.button
        .addClass "clicked"
      jQuery @_floorButton[1 - floor].button
        .removeClass "clicked"

      @mapData.activeFloor = floor
      @updateState()
      @_map.setView(@mapData.currentCenter[floor], @mapData.currentZoom[floor])
      @

  updateState: =>
      floor = @mapData.activeFloor
      @_map.removeLayer(@_exhibits[1 - floor])
      @_map.removeLayer(@_floorLayer[1 - floor])
      @_map.addLayer(@_floorLayer[floor])
      @_map.addLayer(@_exhibits[floor])
      if @_labelsButton._currentState.stateName is 'removeLabels' #setLabels is active
        @_exhibits[1 - floor].eachLayer((layer) ->
          layer.hideLabel()
        )
        @_exhibits[floor].eachLayer((layer) ->
          layer.showLabel()
        )
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

  flyToExhibit: (frame) =>
    return unless frame?
    floor = @mapData.activeFloor
    bounds = new L.LatLngBounds(
      @_map.unproject([frame.x, frame.y], @_maxZoom[floor]),
      @_map.unproject([frame.x + frame.width, frame.y + frame.height], @_maxZoom[floor]),
    )
    if floor isnt frame.mapLevel
      @_floorLayer[frame.mapLevel].getLayers()[0].once("load", =>
        @_map.flyToBounds(bounds, animate: false)
      )
      @setFloorLayer(frame.mapLevel)(@_floorButton[frame.mapLevel]) unless floor is frame.mapLevel
    else
      @_map.flyToBounds(bounds, animate: false)
    return
