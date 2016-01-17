root = exports ? this
root.MutableCanvas = class MutableCanvas extends root.Canvas
  constructor: ->
    super
    @_changeMapButton = L.easyButton('fa-map-o',
                                    => @fireEvents "mapChangeRequest",
                                    'Zmień obrazek piętra',
                                    {position: 'bottomleft'}).addTo @_map

    @_enableResizingButton = L.easyButton({
        states:[
          {
            stateName: 'enableResizing'
            icon: 'fa-arrows-alt'
            title: 'Wyłącz rozciąganie eksponatów'
            onClick: (btn) =>
              jQuery(btn.button).addClass "clicked"
              btn.state('disableResizing')
              @_exhibits[@mapData.activeFloor].eachLayer((layer) ->
                layer.editing.enable()
              )
          },
          {
            stateName: 'disableResizing'
            icon: 'fa-arrows-alt'
            title: 'Włącz rozciąganie eksponatów'
            onClick: (btn) =>
              jQuery(btn.button).removeClass "clicked"
              btn.state('enableResizing')
              @_exhibits[@mapData.activeFloor].eachLayer((layer) ->
                layer.editing.disable()
              )
          }
        ]
    }).addTo @_map
    @_enableResizingButton.state 'enableResizing'

  updateState: =>
    super
    floor = @mapData.activeFloor
    if @_enableResizingButton?._currentState.stateName is 'disableResizing'
      @_exhibits[floor].eachLayer((layer) ->
        layer.editing.enable()
      )
    if @_enableResizingButton?._currentState.stateName is 'enableResizing'
      @_exhibits[floor].eachLayer((layer) ->
        layer.editing.disable()
      )

  _exhibitOptions: (options...) =>
    jQuery.extend(options..., { draggable: true })

  _prepareExhibit: (exh) =>
    exh.editing.enable() if @_enableResizingButton?._currentState.stateName is 'disableResizing'
    exh.on('editstart', @_onEditStart)
    exh.on('edit', @_onEditEnd)
    exh.on('dragstart', @_onDragStart)
    exh.on('dragend', @_onDragEnd)
    return

  _onEditStart: (e) ->
    exhibit = e.target
    exhibit.hideLabel()
    exhibit.bringToFront()
    return

  _onEditEnd: (e) =>
    exhibit = e.target
    @_fixExhibitPosition(exhibit)
    exhibit.editing._repositionCornerMarkers()
    @_updateExhibitPosition(exhibit)
    if @_labelsButton._currentState.stateName is 'removeLabels'
      exhibit.showLabel()
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
    @_updateExhibitPosition(exhibit)
    if @_labelsButton._currentState.stateName is 'removeLabels'
      exhibit.showLabel()
    if @_enableResizingButton._currentState.stateName is 'disableResizing'
      exhibit.editing.enable()
    return

  _updateExhibitPosition: (exhibit) =>
    geoPoints = @_fixExhibitPosition(exhibit)
    scaledPoints = (@_map.project(p, @mapData.maxZoom[@mapData.activeFloor]) for p in geoPoints)
    [topLeft, bottomRight] = [@_getTopLeft(scaledPoints), @_getBottomRight(scaledPoints)]
    @_changeExhibitPositionRequest(exhibit.options.id, topLeft, bottomRight)
    return

  _fixExhibitPosition: (exhibit) =>
    maxX = @_map.project(@_mapBounds[@mapData.activeFloor].getNorthEast()).x
    maxY = @_map.project(@_mapBounds[@mapData.activeFloor].getSouthWest()).y

    latLng = exhibit.getLatLngs()[0]
    exhibitPoints = (@_map.project(ll) for ll in latLng)
    #update points in edge cases
    [topLeft, bottomRight] = [@_getTopLeft(exhibitPoints), @_getBottomRight(exhibitPoints)]
    [dx, dy] = @_getExhibitProtrusion(topLeft, bottomRight, maxX, maxY)
    exhibitPoints = (new L.Point(p.x + dx, p.y + dy) for p in exhibitPoints)
    newGeoPoints = (@_map.unproject(p) for p in exhibitPoints)
    exhibit.setBounds(newGeoPoints)
    return newGeoPoints

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

  _changeExhibitPositionRequest: (id, topLeft, bottomRight) =>
    toSend = {
      jsonData:
        JSON.stringify(
          id: id
          x: topLeft.x
          y: topLeft.y
          width: bottomRight.x - topLeft.x
          height: bottomRight.y - topLeft.y
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
      @mapData.exhibits[data.id].frame.width = data.width
      @mapData.exhibits[data.id].frame.height = data.height
    else
      @refresh()
      BootstrapDialog.alert(
        message: '<p align="center">Wystąpił nieoczekiwany błąd. Spróbuj ponownie.</p>'
        type: BootstrapDialog.TYPE_DANGER
        title: 'Błąd serwera'
      )
    return

  _getTopLeft: (arr) ->
    top = arr[0]
    (top = p if p.x <= top.x and p.y <= top.y) for p in arr[1..]
    top

  _getBottomRight: (arr) ->
    bot = arr[0]
    (bot = p if p.x >= bot.x and p.y >= bot.y) for p in arr[1..]
    bot
