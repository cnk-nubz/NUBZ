root = exports ? this
root.Canvas = class Canvas extends root.View
  constructor: (@_containerMap) ->
    super @_containerMap
    @mapData = new MapDataHandler()
    @_minZoom = 1
    @_maxZoom = [@mapData.floorTilesInfo[0].length, @mapData.floorTilesInfo[1].length]
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
              btn.state('removeLabels')
              @_exhibits[@mapData.activeFloor].eachLayer((layer) ->
                layer.showLabel()
              )
              return
          },
          {
            stateName: 'removeLabels'
            icon: 'fa-comment'
            title: 'Ukryj etykiety'
            onClick: (btn) =>
              btn.state('setLabels')
              @_exhibits[@mapData.activeFloor].eachLayer((layer) ->
                layer.hideLabel()
              )
              return
          }
      ]
    })
    @_labelsButton.state 'removeLabels'
    @_labelsButton.addTo @_map
    @_floorButton = [
      L.easyButton('<strong>0</strong>', @setFloorLayer 0, 'Piętro 0').addTo @_map
      L.easyButton('<strong>1</strong>', @setFloorLayer 1, 'Piętro 1').addTo @_map
    ]
    return

  _init: =>
    mapWidth = (@mapData.floorTilesInfo[i][-1..][0].scaledWidth for i in [0..1])
    mapHeight = (@mapData.floorTilesInfo[i][-1..][0].scaledHeight for i in [0..1])
    for i in [0..1]
      @addMapBounds(i, [0, mapHeight[i]], [mapWidth[i], 0])
      @addFloorLayer(i, @mapData.floorTilesInfo[i], @mapData.floorUrl[i])
      @addExhibits(i, @mapData.visibleExhibits[i])
    @setFloorLayer(@mapData.activeFloor)(@_floorButton[@mapData.activeFloor])
    @

  addMapBounds: (floor, northEast, southWest) =>
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

  setFloorLayer: (floor) =>
    (btn) =>
      jQuery btn.button
        .addClass "clicked"
      jQuery @_floorButton[1 - floor].button
        .removeClass "clicked"

      @mapData.activeFloor = floor
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
      @_map.setView(@_map.unproject([0, 0], 1), 1)
      @_map.invalidateSize()
      @

  addExhibits: (floor, exhibits) =>
    for e in exhibits
      X = e.frame.x
      Y = e.frame.y
      polygonBounds = [
        @_map.unproject([X, Y], 3),
        @_map.unproject([X, Y + e.frame.width], 3),
        @_map.unproject([X + e.frame.height, Y + e.frame.height], 3),
        @_map.unproject([X + e.frame.height, Y], 3)
      ]
      r = L.polygon(polygonBounds, {
          color: "#ff7800"
          weight: 1
        }).bindLabel(e.name, {
          noHide: true
          direction: 'auto'
        })
      @_exhibits[floor].addLayer(r)
    @

  refreshTiles: =>
    floor = @mapData.activeFloor
    tileInfo = @mapData.floorTilesInfo[floor]
    rand = Math.floor(Math.random() * 1024)
    newUrl = "#{@mapData.floorUrl[floor]}?t=#{rand}"
    @_floorLayer[floor].clearLayers()
    @addMapBounds(floor, [0, tileInfo[-1..][0].scaledHeight], [tileInfo[-1..][0].scaledWidth, 0])
    @addFloorLayer(floor, tileInfo, newUrl)
    @setFloorLayer(floor)(@_floorButton[floor])
    @

  refresh: =>
    #TODO (in case nothing to do): add synchronization of panning between pages
    @refreshTiles()
    @
