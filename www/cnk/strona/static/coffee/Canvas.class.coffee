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
    @_labelsButton.addTo @_map
    return

  _init: =>
    actv = @mapData.activeFloor
    @loadData i for i in [1-actv..actv]
    @

  loadData: (floor) =>
    tileInfo = @mapData.floorTilesInfo[floor]
    rand = Math.floor(Math.random() * 1024)
    newUrl = "#{@mapData.floorUrl[floor]}?t=#{rand}"
    @_floorLayer[floor].clearLayers()
    @_exhibits[floor].clearLayers()
    @addMapBounds(floor, [0, tileInfo[-1..][0].scaledHeight], [tileInfo[-1..][0].scaledWidth, 0])
    @addFloorLayer(floor, tileInfo, newUrl)
    @addExhibits(floor, @mapData.exhibits)
    @setFloorLayer(floor)(@_floorButton[floor])
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

  addExhibits: (floor, exhibits) =>
    for idx, e of exhibits
      continue unless e.frame?.mapLevel is floor
      X = e.frame.x
      Y = e.frame.y
      polygonBounds = new L.LatLngBounds(
        @_map.unproject([X, Y], @_maxZoom[floor]),
        @_map.unproject([X + e.frame.width, Y + e.frame.height], @_maxZoom[floor]),
      )
      r = L.rectangle(polygonBounds, jQuery.extend(
        @appearance.exhibit,
        { id: idx }
      )).bindLabel(e.name, {
          direction: 'auto'
        })
      @_exhibits[floor].addLayer(r)
    @

  setFloorLayer: (floor) =>
    (btn) =>
      jQuery btn.button
        .addClass "clicked"
      jQuery @_floorButton[1 - floor].button
        .removeClass "clicked"

      @mapData.activeFloor = floor
      @_updateState()
      @

  _updateState: =>
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
      @_map.setView(@_map.unproject([0, 0], 1), 1)
      @_map.invalidateSize()
      @

  refresh: =>
    #TODO (in case nothing to do): add synchronization of panning between pages
    @loadData(@mapData.activeFloor)
    @
