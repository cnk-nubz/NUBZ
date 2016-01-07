root = exports ? this
root.EditMapPage = class EditMapPage extends root.Controller
  constructor: ->
    super
    @mapData = new MapDataHandler()
    @canvas = new Canvas("#editMap", @_containerId)
    @_init()

  _init: =>
    @changeMapButton = L.easyButton('fa-map-o',
                                    @_showChangeMapPopup,
                                    'Zmień obrazek piętra',
                                    {position: 'bottomleft'}).addTo(@canvas._map)

  refresh: =>
    @canvas.refresh()
    @

  _showChangeMapPopup: =>
    formHTML = """
    <div><form id="dialogForm" style="width: 268px;"><input id="dialogImage" class="form-control" type="file" name="image" required="true" accept="image/*" /><input type="hidden" value="!activeFloor!" name="floor" /><input id="dialogSubmit" type="submit" style="display: none;" /></form></div>
    """
    instance = @
    BootstrapDialog.show(
      message: formHTML.replace '!activeFloor!', "#{@mapData.activeFloor}"
      title: 'Wybierz plik do zmiany mapy'
      closable: false
      size: BootstrapDialog.SIZE_SMALL
      buttons: [{
        label: 'Zamknij',
        action: (dialog) ->
          dialog.close();
      },
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

              success: (data) ->
                errorData = [
                  {"message": "Mapa piętra została pomyślnie zmieniona", "type": BootstrapDialog.TYPE_SUCCESS, "title": "Sukces"}
                  {"message": "Niepoprawny format. Obsługiwane rozszerzenia: .png .jpg .gif .bmp", "type": BootstrapDialog.TYPE_INFO, "title": "Zły format"}
                  {"message": "Wystąpił wewnętrzny błąd serwera - spróbuj ponownie za chwilę. Sprawdź czy serwer jest włączony", "type": BootstrapDialog.TYPE_DANGER, "title": "Błąd serwera"}
                  {"message": "form error - not POST method", "type": BootstrapDialog.TYPE_DANGER, "title": "not POST method"}
                ]
                if data.err == 1
                  data.floorTilesInfo[data.floor] = jQuery.map(data.floorTilesInfo[data.floor], (val) -> [val])
                  instance.mapData.floorTilesInfo[data.floor] = data.floorTilesInfo[data.floor]

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
              )
            e.preventDefault()
            return
          )
          jQuery("#dialogSubmit").click()

      }]
    )
