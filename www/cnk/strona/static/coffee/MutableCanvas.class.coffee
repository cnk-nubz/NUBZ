root = exports ? this
root.MutableCanvas = class MutableCanvas extends root.Canvas
  constructor: ->
    super
    @changeMapButton = L.easyButton('fa-map-o',
                                    => @fireEvents "mapChangeRequest",
                                    'Zmień obrazek piętra',
                                    {position: 'bottomleft'}).addTo(@_map)

  addExhibits: (floor, exhibits) =>
    for idx, e of exhibits
      continue unless e.frame?.mapLevel is floor
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
          id: idx
        }).bindLabel(e.name, {
          direction: 'auto'
        })
      r.on('dragstart', @_onDragStart)
      #IDEA (in case nothing to do): move bouncing from boundaries to @_onDrag
      # plus somehow refresh event latlangs in @_onDrag (leaflet or L.Path.Drag problem?)
      r.on('dragend', @_onDragEnd)
      @_exhibits[floor].addLayer(r)
    @

  _onDragStart: (e) ->
    e.target.hideLabel()
    e.target.bringToFront()
    @

  _onDragEnd: (e) =>
    map = e.target._map
    newLatLng = e.target.getLatLngs()[0]
    maxX = map.project(map.getBounds()._northEast).x
    maxY = map.project(map.getBounds()._southWest).y
    #points are sorted in clockwise order, starting with top left point
    exhibitPoints = (map.project(ll) for ll in newLatLng)
    [dx, dy] = @_getExhibitProtrusion(exhibitPoints[0], exhibitPoints[2], maxX, maxY)
    #update points in case of overflow
    exhibitPoints = (new L.Point(p.x + dx, p.y + dy) for p in exhibitPoints)
    polygonBounds = (map.unproject(p) for p in exhibitPoints)
    e.target.setLatLngs(polygonBounds)
    #scale point back with respect to maxZoom
    exhibitId = e.target.options.id
    geoPoint = map.unproject(exhibitPoints[0])
    scaledPoint = map.project(geoPoint, @mapData.maxZoom[@mapData.activeFloor])
    @mapData.exhibits[exhibitId].frame.x = scaledPoint.x
    @mapData.exhibits[exhibitId].frame.y = scaledPoint.y
    if @_labelsButton._currentState.stateName is 'removeLabels'
      e.target.showLabel()
    @

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
