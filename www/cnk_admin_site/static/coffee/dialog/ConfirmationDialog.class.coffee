root = exports ? this
root.ConfirmationDialog = class ConfirmationDialog extends root.View
  # constructor :: String -> Context
  constructor: (@_data) ->
    super()
    BootstrapDialog.show(
      message: @_data.utils.text.confirmationMessage
      title: @_data.utils.text.confirmationTitle
      closable: false
      buttons:
        [
          @_cancelButton()
        , @_confirmButton()
        ]
    )

  # _cancelButton :: () -> BootstrapDialogButton
  _cancelButton: =>
    label: @_data.utils.text.cancelButton
    action: (dialog) ->
      dialog.close()


  # _confirmButton :: () -> BootstrapDialogButton
  _confirmButton: =>
    label: @_data.utils.text.confirmationButton
    action: (dialog) =>
      @fireEvents('confirm')
      dialog.close()
