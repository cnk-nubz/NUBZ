root = exports ? this
class SimpleQuestionDialog extends root.QuestionDialog
  _dialogCreated: =>
    radioGroup = @_data.data[2][1].radioGroup
    instance = this
    jQuery "#dialog label.#{radioGroup}"
      .filter ":first"
      .addClass "active"

    jQuery "#dialog input[type=text]"
      .each( ->
        jQuery(this).parent().next().css("color", instance._data.utils.style.inputErrorColor)
        jQuery(this).keyup( (e) ->
          obj = jQuery(this)
          regex = new RegExp(instance._data.utils.regex.input)
          if not obj.val().match regex
              instance._showInputError(obj.parent().next(), instance._data.utils.text.inputError)
          else
            obj.parent().next().html("")
        )
      )
    return

new SimpleQuestionDialog("getSimpleQuestionDialog/", "#simpleQuestion")
