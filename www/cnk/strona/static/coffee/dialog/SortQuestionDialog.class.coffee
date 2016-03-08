root = exports ? this
root.SortQuestionDialog = class SortQuestionDialog extends root.QuestionDialog
  _prepareDialog: (dialog) =>
    super
    inputOffset = @_data.utils.default.labelSize
    instance = this

    inputs = jQuery("input[type=text]", dialog)
    inputs.each( (idx) ->
        obj = jQuery(this)
        error = obj.parent().next()
        error.css("color", instance._data.utils.style.inputErrorColor)
        regex = new RegExp(instance._data.utils.regex.input)
        jQuery(this).keyup((e) -> instance._inputKeyUp(regex)(obj, e))
      )
    lastInput = inputs.filter(":last")
    lastInput.parent().addClass("input-group")
    regex = new RegExp(instance._data.utils.regex.dynamicInput)
    lastInput.dynamicInputs(inputOffset, @_inputKeyUp(regex), instance)
    return

  _prepareFilledDialog: (dialog) =>
    jQuery(".form-group:eq(0) input", dialog).val(@_dialogInfo.name)
    jQuery(".form-group:eq(1) input", dialog).val(@_dialogInfo.question)
    for answer, index in @_dialogInfo.options
      jQuery(".form-group:last-child > div input:last", dialog).val(answer).keyup()
    if @readonly
      jQuery("input", dialog).prop("readonly", true)
      # remove last "add answer" entry
      jQuery("input", dialog).last().parents('.input-group').remove()
      @_dialog.getButton('saveButtonDialog').hide()
    @

  _inputKeyUp: (regex) =>
      (obj, e) =>
        error = obj.parent().next()
        if not obj.val().match regex
          if obj.val().length
            @_showInputError(error, @_data.utils.text.inputError)
          else
            @_showInputError(error, @_data.utils.text.emptyInputError)
        else
          error.html("")
        return

  _validateForm: =>
    isValid = true
    instance = this
    jQuery "#dialog input[type=text]:not(:last)"
      .each( ->
        text = jQuery(this).val()
        regex = new RegExp(instance._data.utils.regex.input)
        if not text.match regex
          isValid = false
          error = jQuery(this).parent().next()
          if text.length
            instance._showInputError(error, instance._data.utils.text.inputError)
          else
            instance._showInputError(error, instance._data.utils.text.emptyInputError)
      )
    #check if there is any answer given
    group = jQuery("#dialog .form-group").filter(":last")
    inputs = jQuery("input", group)
    if inputs.length < 3
      isValid = false
      error = inputs.parent().last().next()
      instance._showInputError(error, @_data.utils.text.needMultipleAnswerError)
    isValid

  extractData: =>
    name = jQuery("#dialog .form-group:eq(0) input").val()
    question = jQuery("#dialog .form-group:eq(1) input").val()
    options = []
    jQuery("#dialog .form-group:last-child .input-group input:not(:last)").each( ->
      options.push jQuery(this).val()
    )
    data =
      name: name
      question: question
      options: options
    data
