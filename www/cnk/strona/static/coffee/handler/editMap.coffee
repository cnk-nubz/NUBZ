root = exports ? this
root.cachedData = {}
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
    @_mapData.minZoom = 1
    @_mapData.maxZoom = @_mapData.floorTilesInfo.map((x) -> Math.max(Object.keys(x).length, 1))
    @_exhibitEditDialog = new root.ExhibitDialog('getHTML?name=exhibitDialog', @updateExhibitRequest)
      .setDeleteHandler(@removeExhibitSuccess)
    @_DOM =
      plusZoom: "#zoomControls button:first-child"
      minusZoom: "#zoomControls button:last-child"
      floorButton: "#changeFloor button"
      labels: "#showLabels button"
      resize: "#changeResizing button"
      changeMap: "#changeMap button"

    @canvas = new root.MutableCanvas('#map', @_mapData)
    @panel = new root.ExhibitPanel('#exhibit-panel', @_mapData)
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
    @panel.on("addExhibit", (data, dialog) =>
      @newExhibitRequest(data, dialog)
    )
    @panel.on("flyToExhibitWithId", (id) =>
      exhibit = @_mapData.exhibits[id]
      exhibitFloor = exhibit.frame.mapLevel
      jQuery(@_getNthFloor(@_mapData.activeFloor)).removeClass "active"
      jQuery(@_getNthFloor(exhibitFloor)).addClass "active"
      @canvas.flyToExhibit(id, @_mapData.activeFloor)
      @_mapData.activeFloor = exhibitFloor
      @panel.filterForOneFloor(@_mapData.activeFloor)
    )
    @panel.on("modifyExhibitWithId", (id, dialog) =>
      exhibit = @_mapData.exhibits[id]
      data =
        id: id
        name: exhibit.name
        floor: exhibit.frame?.mapLevel ? @_mapData.numberOfFloors
        color: exhibit.colorHex
      @_exhibitEditDialog.bindData(data).show()
    )
    @canvas.on("zoomChange", (activeZoom) =>
      maxZoom = @_mapData.maxZoom[@_mapData.activeFloor]
      minZoom = @_mapData.minZoom
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
    jQuery.getJSON('getHTML?name=changeMapDialog', floor: activeFloor, (data) ->
      new root.ChangeMapDialog(data,
        activeFloor: activeFloor
        numberOfFloors: numberOfFloors
      ) .on("removeFloor", -> location.reload())
        .on("addFloor", e.data.instance.changeMapSubmitHandler)
        .on("changeFloorMap", e.data.instance.changeMapSubmitHandler)
    )
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
      success: (data) ->
        @afterMapUploadMessage(data)
    )
    return


  # afterMapUploadMessage :: (DialogData) -> undefined
  afterMapUploadMessage: (data) ->
    dialogType =
      success: BootstrapDialog.TYPE_SUCCESS
      danger: BootstrapDialog.TYPE_DANGER
      info: BootstrapDialog.TYPE_INFO

    jQuery.each(BootstrapDialog.dialogs, (id, dialog) ->
      dialog.close()
    )
    BootstrapDialog.show(
      message: data.dialog.message
      title: data.dialog.title ? 'Błąd'
      type: dialogType[data.dialog.type] ? dialogType.danger
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
      frame =
        x: topLeft.x
        y: topLeft.y
        width: viewportWidth
        height: viewportHeight
        mapLevel: @_mapData.activeFloor
    toSend =
      jsonData:
        JSON.stringify(
          name: data.name
          rgbHex: data.rgbHex
          floor: data.floor if data.floor?
          visibleMapFrame: frame
        )
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.post('/createNewExhibit/', toSend, (recvData) =>
      if not recvData.success
        if recvData.exceptionType is "DuplicateName"
          dialog.showNameDuplicatedError()
        else
          BootstrapDialog.alert(
            message: "#{recvData.message}"
            type: BootstrapDialog.TYPE_DANGER
            title: 'Błąd serwera'
          )
        return
      @_newExhibitSuccess(recvData, dialog)
      dialog.close()
    , 'json')
    return


  # _newExhibitSuccess :: Exhibit -> undefined
  _newExhibitSuccess: (data) =>
    id = data.id
    @_mapData.exhibits[id] = {name: null, colorHex: null, frame: {}}
    @_mapData.exhibits[id].name = data.name
    @_mapData.exhibits[id].colorHex = data.rgbHex
    @_mapData.exhibits[id].id = id
    if data.frame?
      @_mapData.exhibits[id].frame.x = data.frame.x
      @_mapData.exhibits[id].frame.y = data.frame.y
      @_mapData.exhibits[id].frame.width = data.frame.width
      @_mapData.exhibits[id].frame.height = data.frame.height
      @_mapData.exhibits[id].frame.mapLevel = data.frame.mapLevel
      @canvas.addExhibits([@_mapData.exhibits[id]])

    jQuery.getJSON('/getAllExhibits', null, (data) =>
      if not data.success
        BootstrapDialog.alert(
          message: data.message
          type: BootstrapDialog.TYPE_DANGER
          title: 'Błąd serwera'
        )
        return
      if data.exhibits?
        @panel.replaceExhibits((e.id for e in data.exhibits))
    )
    return


  # updateExhibitRequest :: (ExhibitRequest, BootstrapDialog) -> undefined
  updateExhibitRequest: (data, dialog) =>
    if data.floor?
      [topLeft, viewportWidth, viewportHeight] = @canvas.getVisibleFrame(@_mapData.activeFloor)
      frame =
        x: topLeft.x
        y: topLeft.y
        width: viewportWidth
        height: viewportHeight
        mapLevel: @_mapData.activeFloor
    toSend =
      jsonData:
        JSON.stringify(
          id: data.id
          rgbHex: data.rgbHex
          floor: data.floor if data.floor?
          visibleMapFrame: frame
        )
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.post('/updateExhibit/', toSend, (recvData) =>
      if not recvData.success
        if recvData.exceptionType is "DuplicateName"
          dialog.showNameDuplicatedError()
        else
          BootstrapDialog.alert(
            message: "#{recvData.message}"
            type: BootstrapDialog.TYPE_DANGER
            title: 'Błąd serwera'
          )
        return
      @_updateExhibitSuccess(recvData)
      dialog.close()
    , 'json')
    return

  # _updateExhibitSuccess :: Exhibit -> undefined
  _updateExhibitSuccess: (data) =>
    @canvas.removeExhibit(data.id)
    @_mapData.exhibits[data.id] = {
      id: data.id
      frame: data.frame
      name: data.name
      colorHex: data.rgbHex
    }
    if data.frame?
      @canvas.addExhibits([@_mapData.exhibits[data.id]])
    @panel.refreshExhibitsList()
    return


  # removeExhibitSuccess :: Int -> Context
  removeExhibitSuccess: (exhibitId) =>
    @canvas.removeExhibit(exhibitId)
    @panel.removeExhibit(exhibitId)
    @

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
