root = exports ? this
root.EditableCanvas = class EditableCanvas extends root.Canvas
  constructor: ->
    super
    @changeMapButton = L.easyButton('fa-map-o',
                                    @_showChangeMapPopup,
                                    'Zmień obrazek piętra',
                                    {position: 'bottomleft'}).addTo(@_map)

  _init: =>
    super

  _showChangeMapPopup: =>
    formHTML = """
    <div><form id="dialogForm" style="width: 268px;"><input id="dialogImage" class="form-control" type="file" name="image" required="true" accept="image/*" /><input type="hidden" value="!activeFloor!" name="floor" /><input id="dialogSubmit" type="submit" style="display: none;" /></form></div>
    """
    instance = @
    BootstrapDialog.show(
      message: formHTML.replace '!activeFloor!', "#{@_activeFloor}"
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
          instance.fireEvents "submit"
          jQuery("#dialogSubmit").click()

      }]
    )
