root = exports ? this
root.Canvas = class Canvas
  constructor: (@_minZoom, @_maxZoom, @_activeFloor) ->
    @_map = L.map('map', {
      minZoom: @_minZoom
      maxZoom: @_maxZoom
      zoom: @_minZoom
      crs: L.CRS.Simple
    })
    @_mapBounds = [null, null]
    @_exhibits = [new L.LayerGroup(), new L.LayerGroup()]
    @_floorLayer = [new L.LayerGroup(), new L.LayerGroup()]
    @_initButtons()



  addMapBounds: (floor, northEast, southWest) =>
    @_mapBounds[floor] = new L.LatLngBounds(@_map.unproject(northEast, @_maxZoom),
                                            @_map.unproject(southWest, @_maxZoom))
    return



  addFloorLayer: (floor, tileInfo, url) =>
    len = tileInfo.length
    for j in [0...Math.min(len, @_maxZoom)]
      zoomLayer = L.tileLayer(url.replace('{z}', "#{j + @_minZoom}"), {
          minZoom: j + @_minZoom
          maxZoom: j + @_minZoom
          tileSize: tileInfo[j].tileWidth
          continuousWorld: true
          crs: L.CRS.Simple
          bounds: @_mapBounds[floor]
      })
      @_floorLayer[floor].addLayer zoomLayer
    return



  setFloorLayer: (floor) =>
    (btn) =>
      console.log "jestem w setFloorLayer"
      console.log "floor #{floor}"
      jQuery btn.button
        .addClass "clicked"
      jQuery @_floorButton[1 - floor].button
        .removeClass "clicked"

      @_activeFloor = floor
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

      console.log @_mapBounds[floor]
      @_map.setMaxBounds @_mapBounds[floor]
      @_map.setView(@_map.unproject([0, 0], 1), 1)
      return



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
    return



  refreshMap: (floor, tileInfo, url) =>
    console.log tileInfo
    console.log url
    rand = Math.floor(Math.random() * 1024)
    newUrl = "#{url}?t=#{rand}"
    console.log newUrl
    @_floorLayer[floor].eachLayer((layer) ->
      layer.setUrl(newUrl)
    )
    @_floorLayer[floor].clearLayers()
    @addMapBounds(floor, [0, tileInfo[-1..][0].scaledHeight], [tileInfo[-1..][0].scaledWidth, 0])
    @addFloorLayer(floor, tileInfo, newUrl)
    @setFloorLayer(floor)(@_floorButton[floor])


  rescaleToContainer: =>
    @_map.invalidateSize()



  _initButtons: =>
    @_labelsButton = L.easyButton({
      states:[
          {
            stateName: 'setLabels'
            icon: 'fa-comment-o'
            title: 'Pokaż etykiety'
            onClick: (btn) =>
              btn.state('removeLabels')
              @_exhibits[@_activeFloor].eachLayer((layer) ->
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
              @_exhibits[@_activeFloor].eachLayer((layer) ->
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
