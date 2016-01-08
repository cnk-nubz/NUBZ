root = exports ? this
root.EditMapPage = class EditMapPage extends root.View
  constructor: ->
    super
    @mapData = new MapDataHandler()
    @canvas = new Canvas("#{@_containerId}-a")
    @_init()
    @addView("map", @canvas)

  _init: =>
    @changeMapButton = L.easyButton('fa-map-o',
                                    @_showChangeMapPopup,
                                    'Zmień obrazek piętra',
                                    {position: 'bottomleft'}).addTo(@canvas._map)
    leftPanelStyle = {
      "background": "rgba(255, 255, 255, 1)"
      "position": "absolute"
      "left": "0px"
      "width": "46px"
      "height": "100%"
      "z-index": "1029"
    }
    canvasStyle = {
      "position": "relative"
      "margin-left": "46px"
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

    @select("#{@_containerId}-a")
      .style(canvasStyle)
    @


  refresh: =>
    @canvas.refresh()
    @

  _showChangeMapPopup: =>
    formHTML = """
    <div><form id="dialogForm" style="width: 268px;"><input id="dialogImage" class="form-control" type="file" name="image" required="true" accept="image/*" /><input type="hidden" value="!activeFloor!" name="floor" /><input id="dialogSubmit" type="submit" style="display: none;" /></form></div>
    """
    instance = @

    dialogCloseButton = ->
      {
        label: 'Zamknij'
        action: (dialog) ->
          dialog.close()
      }

    ajaxSuccessHandler = (data) ->
      errorData = [
        {"message": "Mapa piętra została pomyślnie zmieniona", "type": BootstrapDialog.TYPE_SUCCESS, "title": "Sukces"}
        {"message": "Niepoprawny format. Obsługiwane rozszerzenia: .png .jpg .gif .bmp", "type": BootstrapDialog.TYPE_INFO, "title": "Zły format"}
        {"message": "Wystąpił wewnętrzny błąd serwera - spróbuj ponownie za chwilę. Sprawdź czy serwer jest włączony", "type": BootstrapDialog.TYPE_DANGER, "title": "Błąd serwera"}
        {"message": "form error - not POST method", "type": BootstrapDialog.TYPE_DANGER, "title": "not POST method"}
      ]
      if data.err == 1
        data.floorTilesInfo[data.floor] = jQuery.map(data.floorTilesInfo[data.floor], (val) -> [val])
        instance.mapData.floorTilesInfo[data.floor] = data.floorTilesInfo[data.floor]
        instance.mapData.floorUrl[data.floor] = data.floorUrl

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
      instance.canvas.refresh()
      jQuery("#dialogSubmit").click()
      return


    dialogSendButton = ->
      {
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
            success: ajaxSuccessHandler
          )
          e.preventDefault()
          return
        )
        jQuery("#dialogSubmit").click()
      }

    dialogButtons = [
      dialogCloseButton()
      dialogSendButton()
    ]

    BootstrapDialog.show(
      message: formHTML.replace '!activeFloor!', "#{@mapData.activeFloor}"
      title: 'Wybierz plik do zmiany mapy'
      closable: false
      size: BootstrapDialog.SIZE_SMALL
      buttons: dialogButtons
    )
