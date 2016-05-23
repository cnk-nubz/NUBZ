root = exports ? this
root.ActionDialog = class ActionDialog extends root.QuestionDialog
  constructor: (dialogData, options = {}) ->
    super(dialogData, options)


  # _deleteButton :: () -> BootstrapDialogButton
  _deleteButton: =>
    jQuery.extend(super,
      action: (dialog) =>
        con = new root.ConfirmationDialog(@_data)
        con.on('confirm', =>
            @fireEvents('delete', @_dialogInfo.actionId)
            dialog.close()
          )
    )


  # _prepareFilledDialog :: DOMNode -> undefined
  _prepareFilledDialog: (dialogBody) =>
    @_dialog.setTitle(@_data.utils.text.title)
    jQuery(".form-group:eq(0) input", dialogBody).val(@_dialogInfo.text)
    if @options.readonly
      jQuery("input", dialogBody).prop("readonly", true)
      @_dialog.getButton('saveButtonDialog').hide()
    return

  ###
  # type ActionData = {
  #   text :: String
  # }
  ###
  # extractData :: () -> ActionData
  extractData: ->
    text = jQuery("#dialog input").val()
    data =
      text: text
    data
