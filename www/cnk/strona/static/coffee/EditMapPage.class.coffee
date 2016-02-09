root = exports ? this
root.EditMapPage = class EditMapPage extends root.View
  constructor: ->
    super
    @mapData = new MapDataHandler()
    @appearance = new Appearance()
    @canvasId = "#{@_containerId}-a"
    @exhibitPanelId = "#{@_containerId}-b"
    @canvas = new MutableCanvas(@canvasId)
    @exhibitPanel = new ExhibitPanel(@exhibitPanelId)
    @canvas.on("mapChangeRequest", @_showChangeMapPopup)
    @exhibitPanel.on("addExhibit", =>
      dialog = new ExhibitDialog(null, @mapData.activeFloor, @_addNewExhibitHandler.bind(this))
      dialog.nameEditable = true
      dialog.show()
    )
    @exhibitPanel.on("flyToExhibit", (exhibit) =>
      @canvas.flyToExhibit exhibit
    )
    @_init()
    @addView("rightPanel", @exhibitPanel)
    @addView("map", @canvas)

  _addNewExhibitHandler: (data) =>
    if data.floor?
      [topLeft, ViewportWidth, ViewportHeight] = @canvas.getVisibleFrame()
      frame =
        x: topLeft.x
        y: topLeft.y
        width: ViewportWidth
        height: ViewportHeight
        mapLevel: @mapData.activeFloor
    toSend = {
      jsonData:
        JSON.stringify(
          name: data.name
          floor: data.floor if data.floor?
          visibleMapFrame: frame
        )
    }
    instance = @
    handler = @_ajaxNewExhibitSuccess
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      dataType: 'json'
      url: '/createNewExhibit/'
      data: toSend
      success: handler.bind(instance)
    )
    return

  _ajaxNewExhibitSuccess: (data) =>
    if not data.success
      BootstrapDialog.alert(
        message: '<p align="center">Wystąpił nieoczekiwany błąd. Spróbuj ponownie.</p>'
        type: BootstrapDialog.TYPE_DANGER
        title: 'Błąd serwera'
      )
      return
    id = data.id
    @mapData.exhibits[id] = {name: null, frame: {}}
    @mapData.exhibits[id].name = data.name
    if data.frame?
      @mapData.exhibits[id].frame.x = data.frame.x
      @mapData.exhibits[id].frame.y = data.frame.y
      @mapData.exhibits[id].frame.width = data.frame.width
      @mapData.exhibits[id].frame.height = data.frame.height
      @mapData.exhibits[id].frame.mapLevel = data.frame.mapLevel
      t = {}
      t[id] = @mapData.exhibits[id]
      @canvas.addExhibits(data.frame.mapLevel, t)
      @exhibitPanel.addExhibits(t)
      if data.frame.mapLevel is @mapData.activeFloor
        @canvas.updateState()
    else
      t = {}
      t[id] = @mapData.exhibits[id]
      @exhibitPanel.addExhibits(t)
    return

  _init: =>
    @_initCss()
    @

  _initCss: =>
    exhibitPanelWidth = "300px"
    leftPanelStyle = {
      "background": @appearance.panel.background
      "position": "absolute"
      "top": @appearance.navbar.height
      "left": "0px"
      "bottom": "0px"
      "width": @appearance.panel.mapControls.width
      "z-index": @appearance.panel.zindex
      "border-right": @appearance.panel.border
    }
    exhibitPanelStyle = {
      "background": @appearance.panel.background
      "position": "absolute"
      "top": @appearance.navbar.height
      "right": "0px"
      "bottom": "0px"
      "width": exhibitPanelWidth
      "z-index": @appearance.panel.zindex
      "border-left": @appearance.panel.border
    }
    canvasStyle = {
      "position": "relative"
      "margin-left": @appearance.panel.mapControls.width
      "margin-right": exhibitPanelWidth
      "height": "100%"
      "overflow": "visible"
    }
    @select(@_containerId)
      .append "div"
      .attr(
        id: "left-panel"
      )
      .style(leftPanelStyle)
      .html "&nbsp;"

    @select(@canvasId)
      .style(canvasStyle)

    @select(@exhibitPanelId)
      .style(exhibitPanelStyle)

  _dialogCloseButton: ->
    {
      label: 'Zamknij'
      action: (dialog) ->
        dialog.close()
    }

  _dialogSendButton: =>
    instance = @
    handler = @_ajaxDialogSuccess
    return {
      label: 'Wyślij'
      action: (dialog) ->
        if jQuery("#dialogImage").val() is ""
          return
        dialog.enableButtons false
        jQuery("#dialogForm").off "submit"
        jQuery("#dialogForm").submit((e) =>
          jQuery.ajaxSetup(
            headers: { "X-CSRFToken": getCookie("csrftoken") }
          )
          jQuery.ajax(
            type: "POST"
            url: "/uploadImage/"
            data: new FormData(jQuery("#dialogForm")[0])
            processData: false
            contentType: false
            success: handler.bind(instance)
          )
          e.preventDefault()
          return
        )
        jQuery("#dialogSubmit").click()
        dialog.setMessage('<p align="center">Trwa przetwarzanie mapy</p>')
        dialog.options.buttons = []
        dialog.updateButtons()
    }

  _ajaxDialogSuccess: (data) =>
    errorData = [
      {"message": "Mapa piętra została pomyślnie zmieniona", "type": BootstrapDialog.TYPE_SUCCESS, "title": "Sukces"}
      {"message": "Niepoprawny format. Obsługiwane rozszerzenia: .png .jpg .gif .bmp", "type": BootstrapDialog.TYPE_INFO, "title": "Zły format"}
      {"message": "Wystąpił wewnętrzny błąd serwera - spróbuj ponownie za chwilę. Sprawdź czy serwer jest włączony", "type": BootstrapDialog.TYPE_DANGER, "title": "Błąd serwera"}
      {"message": "form error - not POST method", "type": BootstrapDialog.TYPE_DANGER, "title": "not POST method"}
    ]
    if data.err == 1
      data.floorTilesInfo[data.floor] = jQuery.map(data.floorTilesInfo[data.floor], (val) -> [val])
      @mapData.floorTilesInfo[data.floor] = data.floorTilesInfo[data.floor]
      @mapData.floorUrl[data.floor] = data.floorUrl

    err = data.err - 1
    #close existing dialog
    jQuery.each(BootstrapDialog.dialogs, (id, dialog) ->
      dialog.close()
    )
    BootstrapDialog.show(
      message: errorData[err].message
      title: errorData[err].title
      type: errorData[err].type
    )
    @canvas.refresh()
    return

  _showChangeMapPopup: =>
    formHTML =
      d3.select(document.createElement("div"))
        .append "form"
          .attr(
            id: "dialogForm"
          )
          .style(
            "width": "268px"
          )
    formHTML.append "input"
        .attr(
          id: "dialogImage"
          type: "file"
          name: "image"
          required: "true"
          accept: "image/*"
        )
        .classed "form-control", true
    formHTML.append "input"
        .attr(
          type: "hidden"
          value: "!activeFloor!"
          name: "floor"
        )
    formHTML.append "input"
        .attr(
          id: "dialogSubmit"
          type: "submit"
        )
        .style(
          "display": "none"
        )

    formHTML = formHTML[0][0].outerHTML
    dialogButtons = [
      @_dialogCloseButton()
      @_dialogSendButton()
    ]

    BootstrapDialog.show(
      message: formHTML.replace '!activeFloor!', "#{@mapData.activeFloor}"
      title: 'Zmiana mapy'
      closable: false
      size: BootstrapDialog.SIZE_SMALL
      buttons: dialogButtons
    )
