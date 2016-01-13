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
    #TODO: add synchronization with server, handle exhibits without appended floor
    addNewExhibitHandler = (data) =>
      exhibit =
        42:
          name: data.name
          frame:
            x: 0
            y: 0
            width: 200
            height: 200
            mapLevel: data.floor
      @canvas.addExhibits(data.floor, exhibit)

    @dialog = new ExhibitDialog(null, null, addNewExhibitHandler)
    @dialog.nameEditable = true
    @exhibitPanel.on("addExhibit", => @dialog.show())
    @_init()
    @addView("rightPanel", @exhibitPanel)
    @addView("map", @canvas)

  _init: =>
    @_initCss()
    @

  _initCss: =>
    leftPanelWidth = "46px"
    exhibitPanelWidth = "250px"
    leftPanelStyle = {
      "background": @appearance.panelBackground
      "position": "absolute"
      "left": "0px"
      "width": "46px"
      "height": "100%"
      "z-index": "1029"
    }
    exhibitPanelStyle = {
      "background": @appearance.panelBackground
      "position": "absolute"
      "width": exhibitPanelWidth
      "right": "0px"
      "height": "100%"
      "z-index": "1029"
    }
    canvasStyle = {
      "position": "relative"
      "margin-left": leftPanelWidth
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
    handler = @_ajaxSuccessHandler
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

  _ajaxSuccessHandler: (data) =>
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
