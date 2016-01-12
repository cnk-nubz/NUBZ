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
      polygonBounds = new L.LatLngBounds(
        @_map.unproject([X, Y], @_maxZoom[floor]),
        @_map.unproject([X + e.frame.height, Y + e.frame.height], @_maxZoom[floor]),
      )
      r = L.rectangle(polygonBounds, {
          color: "#ff7800"
          weight: 1
          draggable: true
          id: idx
        }).bindLabel(e.name, {
          direction: 'auto'
        })
      r.editing.enable()
      r.on('editstart', @_onEditStart)
      r.on('edit', @_onEditEnd) #wtf this event name, speak up leaflet developers
      r.on('dragstart', @_onDragStart)
      #IDEA (in case nothing to do): move bouncing from boundaries to @_onDrag
      # plus somehow refresh event's latlangs in @_onDrag (leaflet or L.Path.Drag problem?)
      r.on('dragend', @_onDragEnd)
      @_exhibits[floor].addLayer(r)
    @

  _onEditStart: (e) ->
    exhibit = e.target
    exhibit.hideLabel()
    exhibit.bringToFront()
    return

  _onEditEnd: (e) =>
    exhibit = e.target
    @_fixExhibitPosition(exhibit)
    exhibit.editing._repositionCornerMarkers()
    #TODO: add synchronization with server -> @_changeExhibitPositionRequest
    return

  _onDragStart: (e) ->
    exhibit = e.target
    exhibit.hideLabel()
    exhibit.editing.disable()
    exhibit.bringToFront()
    return

  _onDragEnd: (e) =>
    return unless e.target._dragMoved
    exhibit = e.target
    @_fixExhibitPosition(exhibit)
    exhibitId = exhibit.options.id
    geoPoint = exhibit.getLatLngs()[0][0]
    scaledPoint = @_map.project(geoPoint, @mapData.maxZoom[@mapData.activeFloor])
    @_changeExhibitPositionRequest(exhibitId, scaledPoint)
    if @_labelsButton._currentState.stateName is 'removeLabels'
      exhibit.showLabel()
    exhibit.editing.enable()
    return

  _fixExhibitPosition: (exhibit) =>
    maxX = @_map.project(@_mapBounds[@mapData.activeFloor].getNorthEast()).x
    maxY = @_map.project(@_mapBounds[@mapData.activeFloor].getSouthWest()).y

    latLng = [exhibit.getBounds().getNorthWest(), exhibit.getBounds().getSouthEast()]
    exhibitPoints = (@_map.project(ll) for ll in latLng)
    #update points in edge cases
    [dx, dy] = @_getExhibitProtrusion(exhibitPoints[0], exhibitPoints[1], maxX, maxY)
    exhibitPoints = (new L.Point(p.x + dx, p.y + dy) for p in exhibitPoints)
    newGeoPoints = (@_map.unproject(p) for p in exhibitPoints)
    exhibit.setBounds(newGeoPoints)
    return

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

  _changeExhibitPositionRequest: (id, topLeft) =>
    toSend = {
      jsonData:
        JSON.stringify(
          id: id
          x: topLeft.x
          y: topLeft.y
        )
    }
    instance = @
    handler = @_ajaxSuccessHandler
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      dataType: 'json'
      url: '/updateExhibitPosition/'
      data: toSend
      success: handler.bind(instance)
    )
    return

  _ajaxSuccessHandler: (data) =>
    if data.success is true
      @mapData.exhibits[data.id].frame.x = data.x
      @mapData.exhibits[data.id].frame.y = data.y
    else
      @refresh()
      BootstrapDialog.alert(
        message: '<p align="center">Wystąpił nieoczekiwany błąd. Spróbuj ponownie.</p>'
        type: BootstrapDialog.TYPE_DANGER
        title: 'Błąd serwera'
      )
    return
