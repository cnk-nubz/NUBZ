root = exports ? this
root.ActionDialog = class ActionDialog extends root.QuestionDialog
  _dialogCreated: =>
    super
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
    return
