root = exports ? this
formHTML = """
<div><form id="dialogForm" style="width: 268px;"><input id="dialogImage" class="form-control" type="file" name="image" required="true" accept="image/*" /><input name="floor" type="hidden" value="0" /><input id="dialogSubmit" type="submit" style="display: none;" /></form></div>
"""
root.EditableCanvas = class EditableCanvas extends root.Canvas
  constructor: ->
    super

  spawn: (selection) =>
    super
    instance = @
    BootstrapDialog.show(
      message: formHTML
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
          jQuery("#dialogForm").off("submit")
          handlers = instance._handler["submit"] ? {}

          jQuery.map(handlers, (h) ->
            console.log "witam tutaj"
            jQuery("#dialogForm").submit(h)
          )
          jQuery("#dialogSubmit").click()
          dialog.enableButtons true
      }]
    )
    @

  destroy: =>
    super
    @

  _showResult: (idx) =>
    BootstrapDialog.show(
      message: 'TODO'
    )
