root = exports ? this
class Handlers
  # constructor :: () -> Context
  constructor: ->
    @mapData = new root.MapDataHandler()
    @exhibitEditDialog = new root.ExhibitDialog('getHTML?name=exhibitDialog', @updateExhibitRequest)
    @_DOM =
      plusZoom: "#zoomControls button:first-child"
      minusZoom: "#zoomControls button:last-child"
      floorButton: "#changeFloor button:first-child"
      labels: "#showLabels button"
      resize: "#changeResizing button"
      changeMap: "#changeMap button"

    @canvas = new root.MutableCanvas('#map')
    @panel = new root.ExhibitPanel('#exhibit-panel')
    @_setButtonHandlers()
    @_setEvents()
    jQuery("#{@_DOM.labels}, #{@_getNthFloor(@mapData.activeFloor)}").addClass "active"
    @canvas.setFloorLayer 0


  # _getNthFloor :: Int -> String
  _getNthFloor: (n) ->
    "#{@_DOM.floorButton} #{
      if n is 0
        ""
      else
        "+ " + "button + ".repeat(n)[..-3]
    }"


  # _setButtonHandlers :: () -> undefined
  _setButtonHandlers: =>
    jQuery(@_DOM.minusZoom).click(canvas: @canvas, @zoomOutHandler)
    jQuery(@_DOM.plusZoom).click(canvas: @canvas, @zoomInHandler)
    instance = this
    floorButtons = @mapData.avaibleFloors.map((n) => jQuery(@_getNthFloor(n)))
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
      exhibit = @mapData.exhibits[id]
      exhibitFloor = exhibit.frame.mapLevel
      if exhibitFloor isnt @mapData.activeFloor
        jQuery(@_getNthFloor(exhibitFloor)).addClass "active"
        jQuery(@_getNthFloor(@mapData.activeFloor)).removeClass "active"
      @canvas.flyToExhibit id
      @panel.filterForCurrentFloor()
    )
    @panel.on("modifyExhibitWithId", (id, dialog) =>
      exhibit = @mapData.exhibits[id]
      data =
        id: id
        name: exhibit.name
        floor: exhibit.frame?.mapLevel
        color: exhibit.colorHex
      @exhibitEditDialog.bindData(data).show()
    )
    @canvas.on("zoomend", (disableMinus, disablePlus) =>
      jQuery(@_DOM.plusZoom).prop("disabled", disablePlus)
      jQuery(@_DOM.minusZoom).prop("disabled", disableMinus)
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
  changeFloorHandler: (floor) ->
    (e) ->
      activeFloor = e.data.instance.mapData.activeFloor
      jQuery(e.data.floorButtons[activeFloor]).removeClass("active")
      jQuery(this).blur().addClass("active")
      e.data.instance.canvas.setFloorLayer(floor)
      e.data.instance.panel.filterForCurrentFloor()
      return


  # showLabelsHandler :: Event -> undefined
  showLabelsHandler: (e) ->
    instance = e.data.instance
    statusNow = instance.changeButtonStatus(jQuery(this))
    instance.canvas.updateLabelsVisibility(statusNow)
    return


  # resizeExhibitsHandler :: Event -> undefined
  resizeExhibitsHandler: (e) ->
    instance = e.data.instance
    statusNow = instance.changeButtonStatus(jQuery(this))
    instance.canvas.changeExhibitResizing(statusNow)
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
    activeFloor = e.data.instance.mapData.activeFloor
    jQuery.getJSON('getHTML?name=changeMapDialog', floor: activeFloor, (data) ->
      new root.ChangeMapDialog(data)
        .on("removeFloor", -> location.reload())
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
      [topLeft, viewportWidth, viewportHeight] = @canvas.getVisibleFrame()
      frame =
        x: topLeft.x
        y: topLeft.y
        width: viewportWidth
        height: viewportHeight
        mapLevel: @mapData.activeFloor
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
      @ajaxNewExhibitSuccess(recvData, dialog)
      dialog.close()
    , 'json')
    return


  # ajaxNewExhibitSuccess :: Exhibit -> undefined
  ajaxNewExhibitSuccess: (data) =>
    id = data.id
    @mapData.exhibits[id] = {name: null, colorHex: null, frame: {}}
    @mapData.exhibits[id].name = data.name
    @mapData.exhibits[id].colorHex = data.rgbHex
    if data.frame?
      @mapData.exhibits[id].frame.x = data.frame.x
      @mapData.exhibits[id].frame.y = data.frame.y
      @mapData.exhibits[id].frame.width = data.frame.width
      @mapData.exhibits[id].frame.height = data.frame.height
      @mapData.exhibits[id].frame.mapLevel = data.frame.mapLevel
      @canvas.addExhibits(data.frame.mapLevel, [id])
      if data.frame.mapLevel is @mapData.activeFloor
        @canvas.updateLabelsVisibility()

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
      [topLeft, viewportWidth, viewportHeight] = @canvas.getVisibleFrame()
      frame =
        x: topLeft.x
        y: topLeft.y
        width: viewportWidth
        height: viewportHeight
        mapLevel: @mapData.activeFloor
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
      @ajaxUpdateExhibitSuccess(recvData)
      dialog.close()
    , 'json')
    return

  # ajaxUpdateExhibitSuccess :: Exhibit -> undefined
  ajaxUpdateExhibitSuccess: (data) =>
    if not data.success
      BootstrapDialog.alert(
        message: "<p align=\"center\">#{data.message}</p>"
        type: BootstrapDialog.TYPE_DANGER
        title: 'Błąd serwera'
      )
      return
    @canvas.removeExhibit(data.id)
    @mapData.exhibits[data.id] = {
      frame: data.frame
      name: data.name
      colorHex: data.rgbHex
    }
    if data.frame?
      @canvas.addExhibits(data.frame.mapLevel, [data.id])
    @canvas.updateLabelsVisibility()
    @panel.refreshExhibitsList()
    return

jQuery(document).ready( ->
  handlers = new Handlers()
)
