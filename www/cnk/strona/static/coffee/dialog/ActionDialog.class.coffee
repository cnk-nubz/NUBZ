root = exports ? this
root.ActionDialog = class ActionDialog extends root.QuestionDialog
  _prepareDialog: (dialog) =>
    super
    instance = this
    jQuery("input[type=text]", dialog)
      .each( ->
        jQuery(this).parent().next().css("color", instance._data.utils.style.inputErrorColor)
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

  _prepareFilledDialog: (dialog) =>
    jQuery(".form-group:eq(0) input", dialog).val(@_dialogInfo.text)
    if @readonly
      jQuery("input", dialog).prop("readonly", true)
      @_dialog.getButton('saveButtonDialog').hide()
    @

  extractData: =>
    text = jQuery("#dialog input").val()
    data =
      text: text
    data
