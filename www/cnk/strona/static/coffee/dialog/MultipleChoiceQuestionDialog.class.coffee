root = exports ? this
class MultipleChoiceQuestionDialog extends root.QuestionDialog
  dynamicInputPattern: /^[a-zA-Z\ ]*$/
  _dialogCreated: =>
    radioGroup = @_data.data[2][1].radioGroup
    inputOffset = @_data.data[3][1].labelSize ? 3
    instance = this
    jQuery "#dialog label.#{radioGroup}"
      .filter ":first"
      .addClass "active"

    inputs = jQuery "#dialog input[type=text]"
    inputs.each( (idx) ->
        obj = jQuery(this)
        jQuery(this).parent().next().css("color", instance._data.utils.style.inputErrorColor)
        jQuery(this).keyup((e) -> instance._inputKeyUp(obj, e, false))
      )
    lastInput = inputs.filter(":last")
    lastInput.parent().addClass("input-group")
    lastInput.dynamicInputs(inputOffset, @_inputKeyUp, instance)
    return

  _inputKeyUp: (obj, e, isDynamic) =>
    if not obj.val().match(if isDynamic then @dynamicInputPattern else @inputPattern)
        @_showInputError(obj.parent().next(), @_data.utils.text.inputError)
    else
      obj.parent().next().html("")
    return

  _validateForm: =>
    isValid = true
    instance = this
    jQuery "#dialog input[type=text]:not(:last)"
      .each( ->
        text = jQuery(this).val()
        if not text.match(instance.inputPattern)
          isValid = false
          instance._showInputError(jQuery(this).parent().next(), instance._data.utils.text.inputError)
      )
    #check if there is any answer given
    group = jQuery("#dialog .form-group").filter(":last")
    inputs = jQuery("input", group)
    if inputs.length is 1 and not inputs.val().length
      isValid = false
      instance._showInputError(inputs.parent().next(), @_data.utils.text.emptyInput)
    isValid

new MultipleChoiceQuestionDialog('getMultipleChoiceQuestionDialog/', "#multipleChoiceQuestion")
