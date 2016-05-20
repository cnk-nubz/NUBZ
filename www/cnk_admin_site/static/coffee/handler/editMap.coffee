root = exports ? this
class Handlers
  # type MapData = {
  #   activeFloor     :: Int,
  #   floorUrl        :: String,
  #   exhibitsList    :: [Exhibit]
  #   floorTilesInfo  :: [SingleFloorTilesInfo]
  #   availableFloors :: [Int]
  #   numberOfFloors  :: Int
  # }
  # constructor :: MapData -> Context
  constructor: (@_mapData) ->
    @_mapData.minZoom = @_mapData.floorTilesInfo.map( -> 1)
    @_mapData.maxZoom = @_mapData.floorTilesInfo.map((x) -> Math.max(Object.keys(x).length, 1))
    @_exhibitEditDialog = new root.ExhibitDialog(root.structures.dialog.exhibit,
      readonly: false
      deletable: true
    , @_mapData.activeFloor)
    .on('save', @updateExhibitRequest)
    .on('delete', @removeExhibitRequest)
    @_DOM =
      plusZoom: "#zoomControls button:first-child"
      minusZoom: "#zoomControls button:last-child"
      floorButton: "#changeFloor button"
      labels: "#showLabels button"
      resize: "#changeResizing button"
      changeMap: "#changeMap button"

    @canvas = new root.MutableCanvas('#map', @_mapData)
    @panel = new root.ExhibitPanel(@_mapData)
    @_setButtonHandlers()
    @_setEvents()
    jQuery("#{@_DOM.labels}, #{@_getNthFloor(@_mapData.activeFloor)}").addClass "active"
    @canvas.setFloorLayer 0, 0


  # _getNthFloor :: Int -> String
  _getNthFloor: (n) ->
    "#{@_DOM.floorButton}:eq(#{n})"


  # _setButtonHandlers :: () -> undefined
  _setButtonHandlers: =>
    jQuery(@_DOM.minusZoom).click(canvas: @canvas, @zoomOutHandler)
    jQuery(@_DOM.plusZoom).click(canvas: @canvas, @zoomInHandler)
    floorButtons = @_mapData.availableFloors.map((n) => jQuery(@_getNthFloor(n)))
    for button, idx in floorButtons
      data =
        instance: this
        floorButtons: floorButtons
      button.click(data, @changeFloorHandler(idx))
    jQuery(@_DOM.labels).click(instance: this, @showLabelsHandler)
    jQuery(@_DOM.resize).click(instance: this, @resizeExhibitsHandler)
    jQuery(@_DOM.changeMap).click(instance: this, @changeMapHandler)
    return


  # _setEvents :: () -> undefined
  _setEvents: =>
    @panel.on("addExhibit", @newExhibitRequest)
    @panel.on("flyToExhibitWithId", (exhibitId) =>
      exhibit = @_mapData.exhibits[exhibitId]
      exhibitFloor = exhibit.mapFrame.floor
      jQuery(@_getNthFloor(@_mapData.activeFloor)).removeClass "active"
      jQuery(@_getNthFloor(exhibitFloor)).addClass "active"
      @canvas.flyToExhibit(exhibitId, @_mapData.activeFloor)
      @_mapData.activeFloor = exhibitFloor
      @panel.filterForOneFloor(@_mapData.activeFloor)
    )
    @panel.on("modifyExhibitWithId", (exhibitId, dialog) =>
      exhibit = @_mapData.exhibits[exhibitId]
      data =
        exhibitId: exhibitId
        name: exhibit.name
        floor: exhibit.mapFrame?.floor ? @_mapData.numberOfFloors
        color: exhibit.rgbHex
      @_exhibitEditDialog.bindData(data).show()
    )
    @canvas.on("zoomChange", (activeZoom) =>
      maxZoom = @_mapData.maxZoom[@_mapData.activeFloor]
      minZoom = @_mapData.minZoom[@_mapData.activeFloor]
      jQuery(@_DOM.plusZoom)
        .prop("disabled", activeZoom >= maxZoom)
      jQuery(@_DOM.minusZoom)
        .prop("disabled", activeZoom <= minZoom or minZoom is maxZoom)
    )
    return


  # zoomOutHandler :: Event -> undefined
  zoomOutHandler: (e) ->
    jQuery(this).blur()
    e.data.canvas.zoomOut()
    return


  # zoomInHandler :: Event -> undefined
  zoomInHandler: (e) ->
    jQuery(this).blur()
    e.data.canvas.zoomIn()
    return


  # changeFloorHandler :: Int -> Event -> undefined
  changeFloorHandler: (newFloor) ->
    (e) ->
      oldFloor = e.data.instance._mapData.activeFloor
      e.data.instance._mapData.activeFloor = newFloor
      jQuery(e.data.floorButtons[oldFloor]).removeClass("active")
      jQuery(this).blur().addClass("active")
      e.data.instance.canvas.setFloorLayer(newFloor, oldFloor)
      e.data.instance.panel.filterForOneFloor(newFloor)
      return


  # showLabelsHandler :: Event -> undefined
  showLabelsHandler: (e) ->
    instance = e.data.instance
    activeFloor = instance._mapData.activeFloor
    statusNow = instance.changeButtonStatus(jQuery(this))
    instance.canvas.setLabelsVisibility(statusNow, activeFloor)
    return


  # resizeExhibitsHandler :: Event -> undefined
  resizeExhibitsHandler: (e) ->
    instance = e.data.instance
    activeFloor = instance._mapData.activeFloor
    statusNow = instance.changeButtonStatus(jQuery(this))
    instance.canvas.setExhibitResizing(statusNow, activeFloor)
    return


  # changeButtonStatus :: jQueryObject -> Boolean
  changeButtonStatus: (button) ->
    button.blur()
    isActive = button.hasClass("active")
    if isActive
      button.removeClass("active")
    else
      button.addClass("active")
    return not isActive


  # changeMapHandler :: Event -> undefined
  changeMapHandler: (e) ->
    activeFloor = e.data.instance._mapData.activeFloor
    numberOfFloors = e.data.instance._mapData.numberOfFloors
    new root.ChangeMapDialog(root.structures.dialog.changeMap,
      activeFloor: activeFloor
      numberOfFloors: numberOfFloors
    ) .on("removeFloor", -> location.reload())
      .on("addFloor", e.data.instance.changeMapSubmitHandler)
      .on("changeFloorMap", e.data.instance.changeMapSubmitHandler)
    return

  # changeMapSubmitHandler :: (Event, FormData) -> undefined
  changeMapSubmitHandler: (e, formData) =>
    e.preventDefault()
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: "POST"
      url: "/uploadImage/"
      context: this
      data: formData
      processData: false
      contentType: false
      error: @_displayError
      success: @afterMapUploadMessage
    )
    return


  # afterMapUploadMessage :: (DialogData) -> undefined
  afterMapUploadMessage: (data) ->
    jQuery.each(BootstrapDialog.dialogs, (id, dialog) ->
      dialog.close()
    )
    BootstrapDialog.show(
      message: data.message
      title: data.title
      type: BootstrapDialog.TYPE_SUCCESS
      closable: false
      buttons: [
        label: 'OK'
        action: -> location.reload()
      ]
    )
    return


  # newExhibitRequest :: (ExhibitRequest, BootstrapDialog) -> undefined
  newExhibitRequest: (data, dialog) =>
    if data.floor?
      [topLeft, viewportWidth, viewportHeight] = @canvas.getVisibleFrame(@_mapData.activeFloor)
      mapFrame =
        frame:
          size:
            width: viewportWidth
            height: viewportHeight
          x: topLeft.x
          y: topLeft.y
        floor: @_mapData.activeFloor
    toSend =
      jsonData:
        JSON.stringify(
          name: data.name
          rgbHex: data.rgbHex
          floor: data.floor
          visibleFrame: mapFrame
        )
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      method: 'POST'
      data: toSend
      dataType: 'json'
      url: '/exhibit/'
      error: @_displayError
      success: (recvData) =>
        @_newExhibitSuccess(recvData, dialog)
        dialog.close()
    )
    return


  # _newExhibitSuccess :: Exhibit -> undefined
  _newExhibitSuccess: (data) =>
    @_mapData.exhibits[data.exhibitId] = data
    if data.mapFrame?.floor is @_mapData.activeFloor
      @canvas.addExhibits([@_mapData.exhibits[data.exhibitId]])
    jQuery.ajax(
      method: "GET"
      dataType: 'json'
      url: '/exhibit/'
      error: @_displayError
      success: (data) =>
        @panel.replaceExhibits((e.exhibitId for e in data.exhibits))
    )
    return


  # updateExhibitRequest :: (ExhibitRequest, BootstrapDialog) -> undefined
  updateExhibitRequest: (data, dialog) =>
    if data.floor?
      [topLeft, viewportWidth, viewportHeight] = @canvas.getVisibleFrame(@_mapData.activeFloor)
      mapFrame =
        frame:
          size:
            width: viewportWidth
            height: viewportHeight
          x: topLeft.x
          y: topLeft.y
        floor: @_mapData.activeFloor
    toSend =
      jsonData:
        JSON.stringify(
          exhibitId: data.exhibitId
          rgbHex: data.rgbHex
          floor: data.floor if data.floor?
          visibleFrame: mapFrame
        )
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      method: "PUT"
      data: toSend
      url: '/exhibit/'
      dataType: 'json'
      error: @_displayError
      success: (recvData) =>
        @_updateExhibitSuccess(recvData)
        dialog.close()
    )
    return

  # _updateExhibitSuccess :: Exhibit -> undefined
  _updateExhibitSuccess: (data) =>
    @canvas.removeExhibit(data.exhibitId)
    @_mapData.exhibits[data.exhibitId] = data
    if data.mapFrame?.floor is @_mapData.activeFloor
      @canvas.addExhibits([@_mapData.exhibits[data.exhibitId]])
    @panel.refreshExhibitsList()
    return


  # removeExhibitRequest :: Int -> Context
  removeExhibitRequest: (exhibitId) =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'DELETE'
      url: '/exhibit/'
      data: (jsonData: JSON.stringify(exhibitId : exhibitId))
      error: @_displayError
      success: =>
        @canvas.removeExhibit(exhibitId)
        @panel.removeExhibit(exhibitId)
    )
    @


  # _displayError :: jqXHR -> undefined
  _displayError: (obj) ->
    BootstrapDialog.show(
      message: obj.responseText
      title: obj.statusText
      type: BootstrapDialog.TYPE_DANGER
    )
    return


jQuery(document).ready( ->
  mapData =
    activeFloor: root.activeFloor
    floorUrl: root.floorUrl
    exhibitsList: root.exhibitsList
    exhibits: root.exhibits
    floorTilesInfo: root.floorTilesInfo
    availableFloors: root.availableFloors
    numberOfFloors: 1 + Math.max.apply(null, root.availableFloors)
  handlers = new Handlers(mapData)
)
