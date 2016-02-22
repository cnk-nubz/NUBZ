root = exports ? this
root.ActionDialog = class ActionDialog extends root.QuestionDialog
  _dialogCreated: =>
    super
    if @_dialogInfo?
      jQuery("#dialog .form-group:eq(0) input").val(@_dialogInfo.text)
    instance = this

    jQuery "#dialog input[type=text]"
      .each( ->
        jQuery(this).parent().next().css("color", instance._data.utils.style.inputErrorColor)
        jQuery(this).keyup( (e) ->
          obj = jQuery(this)
          regex = new RegExp(instance._data.utils.regex.input)
          error = obj.parent().next()
          if not obj.val().match regex
              if obj.val().length
                instance._showInputError(error, instance._data.utils.text.inputError)
              else
                instance._showInputError(error, instance._data.utils.text.emptyInputError)
          else
            obj.parent().next().html("")
        )
      )
    jQuery("#dialog input").prop("readonly", @readonly)
    return

  extractData: =>
    text = jQuery("#dialog input").val()
    data =
      text: text
    data
