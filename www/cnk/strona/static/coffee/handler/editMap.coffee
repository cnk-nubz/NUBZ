root = exports ? this
class Handlers
  constructor: (@canvas, @panel) ->
    @mapData = new root.MapDataHandler()
    @exhibitEditDialog = new root.ExhibitDialog('getHTML?name=exhibitDialog', @updateExhibitRequest)
    @button =
      plusZoom: "#zoomControls button:first-child"
      minusZoom: "#zoomControls button:last-child"
      groundFloor: "#changeFloor button:first-child"
      firstFloor: "#changeFloor button:last-child"
      labels: "#showLabels button"
      resize: "#changeResizing button"
      changeMap: "#changeMap button"
    jQuery(@button.minusZoom).prop "disabled", true
    if @mapData.floorTilesInfo[0].length is 0 and @mapData.floorTilesInfo[1].length is 0
      jQuery(@button.plusZoom).prop "disabled", true
    @_setButtonHandlers()
    @_setEvents()
    if @mapData.activeFloor is 0
      jQuery("#{@button.labels}, #{@button.groundFloor}").addClass "active"
    else
      jQuery("#{@button.labels}, #{@button.firstFloor}").addClass "active"

  _setButtonHandlers: =>
    jQuery(@button.minusZoom).on('click', @zoomOutHandler())
    jQuery(@button.plusZoom).on('click', @zoomInHandler())
    jQuery(@button.groundFloor).on('click', @changeFloorHandler(0))
    jQuery(@button.firstFloor).on('click', @changeFloorHandler(1))
    jQuery(@button.labels).on('click', @showLabelsHandler())
    jQuery(@button.resize).on('click', @resizeExhibitsHandler())
    jQuery(@button.changeMap).on('click', @changeMapHandler())

  _setEvents: =>
    @panel.on("addExhibit", (data, dialog) =>
      @newExhibitRequest(data, dialog)
    )
    @panel.on("flyToExhibitWithId", (id) =>
      exhibit = @mapData.exhibits[id]
      exhibitFloor = exhibit.frame.mapLevel
      if exhibitFloor isnt @mapData.activeFloor
        jQuery("#changeFloor button:eq(#{exhibitFloor})").addClass "active"
        jQuery("#changeFloor button:eq(#{1 - exhibitFloor})").removeClass "active"
      @canvas.flyToExhibit id
      @panel.filterForCurrentFloor()
      jQuery(@button.plusZoom).prop "disabled", true
      jQuery(@button.minusZoom).prop "disabled", false
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
      jQuery(@button.plusZoom).prop("disabled", disablePlus)
      jQuery(@button.minusZoom).prop("disabled", disableMinus)
    )

  zoomOutHandler: =>
    instance = this
    ->
      jQuery(this).blur()
      instance.canvas.zoomOut()

  zoomInHandler: =>
    instance = this
    ->
      jQuery(this).blur()
      instance.canvas.zoomIn()

  changeFloorHandler: (floor) =>
    instance = this
    floorButtons = [jQuery(@button.groundFloor), jQuery(@button.firstFloor)]
    ->
      jQuery(this).blur()
      jQuery(floorButtons[1 - floor]).removeClass "active"
      jQuery(floorButtons[floor]).addClass "active"
      instance.canvas.setFloorLayer(floor)
      instance.panel.refreshDialogInstance()
      instance.panel.filterForCurrentFloor()

  showLabelsHandler: =>
    instance = this
    ->
      statusNow = instance.changeButtonStatus(jQuery(this))
      instance.canvas.changeLabelsVisibility(statusNow)

  resizeExhibitsHandler: =>
    instance = this
    ->
      statusNow = instance.changeButtonStatus(jQuery(this))
      instance.canvas.changeExhibitResizing(statusNow)

  changeButtonStatus: (button) ->
    button.blur()
    isActive = button.hasClass("active")
    if isActive
      button.removeClass("active")
    else
      button.addClass("active")
    return not isActive

  changeMapHandler: =>
    =>
      jQuery.getJSON('getHTML?name=changeMapDialog', floor: @mapData.activeFloor, (data) =>
        @showChangeMapPopup(data.html)
      )

  showChangeMapPopup: (html) =>
    activeFloor = @mapData.activeFloor
    instance = this
    dialog = BootstrapDialog.show(
      message: html
      title: 'Zmiana mapy'
      size: BootstrapDialog.SIZE_SMALL
      closable: false
      buttons: [
        instance.changeMapCancelButton()
        instance.changeMapSaveButton()
      ]
    )

  changeMapCancelButton: ->
    label: 'Anuluj'
    action: (dialog) ->
      dialog.close()

  changeMapSaveButton: =>
    return {
      label: 'Wyślij'
      action: (dialog) =>
        if jQuery("#dialogImage").val() is ""
          return
        dialog.enableButtons false
        jQuery("#dialogForm").off "submit"
        jQuery("#dialogForm").submit(@changeMapSubmitHandler)
        jQuery("#dialogSubmit").click()
        dialog.setMessage('<p align="center">Trwa przetwarzanie mapy</p>')
        dialog.options.buttons = []
        dialog.updateButtons()
    }

  changeMapSubmitHandler: (e) =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: "POST"
      url: "/uploadImage/"
      context: this
      data: new FormData(jQuery("#dialogForm")[0])
      processData: false
      contentType: false
      success: (data) ->
        @afterMapUploadMessage(data)
    )
    e.preventDefault()
    return

  afterMapUploadMessage: (data) =>
    errorData = [
      {"message": "Mapa piętra została pomyślnie zmieniona", "type": BootstrapDialog.TYPE_SUCCESS, "title": "Sukces"}
      {"message": "Niepoprawny format. Obsługiwane rozszerzenia: .png .jpg .gif .bmp", "type": BootstrapDialog.TYPE_INFO, "title": "Zły format"}
      {"message": "Wystąpił wewnętrzny błąd serwera - spróbuj ponownie za chwilę. Sprawdź czy serwer jest włączony", "type": BootstrapDialog.TYPE_DANGER, "title": "Błąd serwera"}
      {"message": "form error - not POST method", "type": BootstrapDialog.TYPE_DANGER, "title": "not POST method"}
    ]
    if data.err == 1
      data.floorTilesInfo[data.floor] = jQuery.map(data.floorTilesInfo[data.floor], (val) -> [val])
      data.exhibits = jQuery.map(data.exhibits, (val) -> [val])
      @mapData.exhibits = {}
      for e in data.exhibits
        @mapData.exhibits[e.id] =
          frame: e.frame
          name: e.name
          colorHex: e.colorHex
      @mapData.floorTilesInfo[data.floor] = data.floorTilesInfo[data.floor]
      @mapData.floorUrl[data.floor] = data.floorUrl
      @mapData.maxZoom[data.floor] = data.floorTilesInfo[data.floor].length
      northEast = [0, data.floorTilesInfo[data.floor][-1..][0].scaledHeight]
      southWest = [data.floorTilesInfo[data.floor][-1..][0].scaledWidth, 0]
      @canvas.addMapBounds(data.floor, northEast, southWest)
      @panel.replaceExhibits((e.id for e in data.exhibits))

    err = data.err - 1
    #close existing dialog
    jQuery.each(BootstrapDialog.dialogs, (id, dialog) ->
      dialog.close()
    )
    BootstrapDialog.show(
      message: errorData[err].message
      title: errorData[err].title
      type: errorData[err].type
      closable: false if err is 2
      buttons: [
        label: 'OK'
        action: -> location.reload()
      ] if err is 2
    )
    @canvas.refresh()
    return

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
    jQuery.ajax(
      type: 'POST'
      dataType: 'json'
      url: '/createNewExhibit/'
      data: toSend
      success: (recvData) =>
        if not recvData.success
          if recvData.exceptionType is "DuplicateName"
            dialog.showNameDuplicatedError()
          else
            BootstrapDialog.alert(
              message: "#{data.message}"
              type: BootstrapDialog.TYPE_DANGER
              title: 'Błąd serwera'
            )
          return
        @ajaxNewExhibitSuccess(recvData, dialog)
        dialog.close()
    )

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
        @canvas.updateState()

    jQuery.getJSON('/getAllExhibits', null, (data) =>
      if not data.success
        BootstrapDialog.alert(
          message: data.message
          type: BootstrapDialog.TYPE_DANGER
          title: 'Błąd serwera'
        )
        return
      @panel.replaceExhibits((e.id for e in data.exhibits))
    )
    return

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
    jQuery.ajax(
      type: 'POST'
      dataType: 'json'
      url: '/updateExhibit/'
      data: toSend
      success: (recvData) =>
        if not recvData.success
          if recvData.exceptionType is "DuplicateName"
            dialog.showNameDuplicatedError()
          else
            BootstrapDialog.alert(
              message: "#{data.message}"
              type: BootstrapDialog.TYPE_DANGER
              title: 'Błąd serwera'
            )
          return
        @ajaxUpdateExhibitSuccess(recvData)
        dialog.close()
    )

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
    @canvas.updateState()
    @panel.refreshExhibitsList()

jQuery(document).ready( ->
  map = new root.MutableCanvas('#map')
  panel = new root.ExhibitPanel('#exhibit-panel')
  handlers = new Handlers(map, panel)
)
