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
    @dialog = new ExhibitDialog(null, @mapData.activeFloor, @_addNewExhibitHandler.bind(this))
    @dialog.nameEditable = true
    @exhibitPanel.on("addExhibit", => @dialog.show())
    @_init()
    @addView("rightPanel", @exhibitPanel)
    @addView("map", @canvas)

  _addNewExhibitHandler: (data) =>
    [topLeft, ViewportWidth, ViewportHeight] = @canvas.getVisibleFrame()
    if data.floor?
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
          floor: data.floor ? null
          visibleMapFrame: frame ? null
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
    return unless data.success
    id = data.id
    @mapData.exhibits[id] ?= {name: null, frame: {}}
    #TODO: change when server will respond
    @mapData.exhibits[id].name = data.name ? "brak nazwy"
    @mapData.exhibits[id].frame.x = data.frame?.x ? 0
    @mapData.exhibits[id].frame.y = data.frame?.y ? 0
    @mapData.exhibits[id].frame.width = data.frame?.width ? 200
    @mapData.exhibits[id].frame.height = data.frame?.height ? 200
    @mapData.exhibits[id].frame.mapLevel = data.frame?.mapLevel ? 0
    t = {}
    t[id] = @mapData.exhibits[id]
    if data.frame?.mapLevel?
      @canvas.addExhibits(data.frame.mapLevel, t)
      @canvas.updateState()
    return

  _init: =>
    @_initCss()
    @

  _initCss: =>
    leftPanelStyle = {
      "background": @appearance.panel.background
      "position": "absolute"
      "top": @appearance.navbar.height
      "left": "0px"
      "bottom": "0"
      "width": @appearance.panel.left.width
      "z-index": "1029"
      "border-right": "1px solid #080808"
    }
    exhibitPanelStyle = {
      "background": @appearance.panel.background
      "position": "absolute"
      "top": @appearance.navbar.height
      "right": "0px"
      "bottom": "0px"
      "width": @appearance.panel.right.width
      "z-index": "1029"
      "border-left": "1px solid #080808"
    }
    canvasStyle = {
      "position": "relative"
      "margin-left": @appearance.panel.left.width
      "margin-right": @appearance.panel.right.width
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
