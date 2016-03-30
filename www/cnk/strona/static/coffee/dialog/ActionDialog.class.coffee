root = exports ? this
root.ActionDialog = class ActionDialog extends root.QuestionDialog
  _prepareDialog: (dialogBody) =>
    super
    instance = this
    jQuery("input[type=text]", dialogBody)
      .each( ->
        jQuery(this).keyup( (e) ->
          obj = jQuery(this)
          text = obj.val()
          error = obj.parent().next()
          if text.length is 0
            instance._showInputError(error, instance._data.utils.text.emptyInputError)
          else
            error.html("")
        )
      )

    return

  _prepareFilledDialog: (dialogBody) =>
    @_dialog.setTitle(@_data.utils.text.title)
    jQuery(".form-group:eq(0) input", dialogBody).val(@_dialogInfo.text)
    if @readonly
      jQuery("input", dialogBody).prop("readonly", true)
      @_dialog.getButton('saveButtonDialog').hide()
    @

  extractData: =>
    text = jQuery("#dialog input").val()
    data =
      text: text
    data
