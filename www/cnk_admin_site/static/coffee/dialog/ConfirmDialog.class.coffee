root = exports ? this
root.ConfirmDialog = class ConfirmDialog
  constructor: (@_url, @_confirmHandler = (->)) ->
    jQuery.getJSON(@_url, null, (data) =>
      @_data = data.data
      @_dialog = new BootstrapDialog(
        message: data.data.utils.text.question
        title: data.data.utils.text.title
        closable: false
        buttons: [@_cancelButton(), @_confirmButton()]
      )
      @_dialog.realize()
      @_dialog.open()
    )

  _cancelButton: =>
    id: 'cancelButtonDialog'
    label: @_data.utils.text.cancelButton
    action: (dialog) ->
      dialog.close()

  _confirmButton: =>
    id: 'confirmButtonDialog'
    label: @_data.utils.text.confirmButton
    action: (dialog) =>
      @_confirmHandler()
      dialog.close()
