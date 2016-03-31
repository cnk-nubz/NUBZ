root = exports ? this
root.SortQuestionDialog = class SortQuestionDialog extends root.QuestionDialog
  _prepareDialog: (dialogBody) =>
    super
    inputOffset = @_data.utils.default.labelSize
    instance = this

    inputs = jQuery("input[type=text]", dialogBody)
    inputs.each( (idx) ->
        obj = jQuery(this)
        jQuery(this).keyup((e) -> instance._inputKeyUp(obj, e))
      )
    lastInput = inputs.filter(":last")
    lastInput.parent().addClass("input-group")
    lastInput.dynamicInputs(inputOffset, @_inputKeyUp, instance)
    return

  _prepareFilledDialog: (dialogBody) =>
    @_dialog.setTitle(@_data.utils.text.title)
    jQuery(".form-group:eq(0) input", dialogBody).val(@_dialogInfo.name)
    jQuery(".form-group:eq(1) input", dialogBody).val(@_dialogInfo.question)
    for answer, index in @_dialogInfo.options
      jQuery(".form-group:last-child > div input:last", dialogBody).val(answer).keyup()
    if @readonly
      jQuery("input", dialogBody).prop("readonly", true)
      # remove last "add answer" entry
      jQuery("input", dialogBody).last().parents('.input-group').remove()
      @_dialog.getButton('saveButtonDialog').hide()
    @

  _inputKeyUp: (obj, e) =>
    text = obj.val()
    error = obj.parent().next()
    if text.length is 0
      @_showInputError(error, @_data.utils.text.emptyInputError)
    else
      error.html("")
    return

  _validateForm: =>
    isValid = true
    instance = this
    jQuery "#dialog input[type=text]:not(:last)"
      .each( ->
        obj = jQuery(this)
        text = obj.val()
        if text.length is 0
          isValid = false
          error = obj.parent().next()
          instance._showInputError(error, instance._data.utils.text.emptyInputError)
      )
    #check if there is any answer given
    group = jQuery("#dialog .form-group").filter(":last")
    inputs = jQuery("input", group)
    if inputs.length < 3
      isValid = false
      error = inputs.parent().last().next()
      @_showInputError(error, @_data.utils.text.needMultipleAnswerError)
    else # check if there are any duplicates in options
      option = {}
      inputs.each( ->
        inputVal = jQuery(this).val()
        if option.hasOwnProperty(inputVal)
          isValid = false
          error = jQuery(this).parent().next()
          instance._showInputError(error, instance._data.utils.text.optionDuplicatedError)
          return
        option[inputVal] = true
      )
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
