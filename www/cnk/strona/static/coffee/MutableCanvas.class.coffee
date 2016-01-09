root = exports ? this
root.MutableCanvas = class MutableCanvas extends root.Canvas
  constructor: ->
    super
    @changeMapButton = L.easyButton('fa-map-o',
                                    => @fireEvents "mapChangeRequest",
                                    'Zmień obrazek piętra',
                                    {position: 'bottomleft'}).addTo(@_map)

  _onDragStart: (e) =>
    e.target.hideLabel()
    e.target.bringToFront()
    @

  _onDragEnd: (e) =>
    map = e.target._map
    newLL = e.target.getLatLngs()[0]
    maxX = map.latLngToContainerPoint(map.getBounds()._northEast).x
    maxY = map.latLngToContainerPoint(map.getBounds()._southWest).y
    topleft = map.latLngToContainerPoint(newLL[0])
    topright = map.latLngToContainerPoint(newLL[1])
    bottomright = map.latLngToContainerPoint(newLL[2])
    bottomleft = map.latLngToContainerPoint(newLL[3])
    if topleft.x < 0
      dx = -topleft.x
    else if bottomright.x > maxX
      dx = maxX - bottomright.x
    else
      dx = 0
    if topleft.y < 0
      dy = -topleft.y
    else if bottomright.y > maxY
      dy = maxY - bottomright.y
    else
      dy = 0
    tl = new L.Point(topleft.x + dx, topleft.y + dy)
    tr = new L.Point(topright.x + dx, topright.y + dy)
    br = new L.Point(bottomright.x + dx, bottomright.y + dy)
    bl = new L.Point(bottomleft.x + dx, bottomleft.y + dy)
    polygonBounds = [
      map.containerPointToLatLng(tl)
      map.containerPointToLatLng(tr)
      map.containerPointToLatLng(br)
      map.containerPointToLatLng(bl)
    ]
    e.target.setLatLngs(polygonBounds)
    if @_labelsButton._currentState.stateName is 'removeLabels'
      e.target.showLabel()
    @_startLatLngs = null
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
          draggable: true
        }).bindLabel(e.name, {
          direction: 'auto'
        })
      r.on('dragstart', @_onDragStart)
      #IDEA (in case nothing to do): move "bouncing" from boundaries to @_onDrag
      # plus somehow refresh event latlangs in @_onDrag
      r.on('dragend', @_onDragEnd)

      @_exhibits[floor].addLayer(r)
    @
