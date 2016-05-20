root = exports ? this
root.MultipleChoiceQuestionDialog = class MultipleChoiceQuestionDialog extends root.QuestionDialog
  constructor: (dialogData, options = {}) ->
    super(dialogData, options)
    @_questionType = 1
  # _prepareDialog :: DOMNode -> undefined
  _prepareDialog: (dialogBody) =>
    super
    #add dialog info
    radioGroup = @_data.utils.default.radioGroup
    inputOffset = @_data.utils.default.labelSize
    instance = this
    if not @_dialogInfo?
      jQuery("label.#{radioGroup}", dialogBody).filter(":first").addClass("active")

    # prepare inputs with regexes etc
    inputs = jQuery("input[type=text]", dialogBody)
    inputs.each( (idx) ->
      obj = jQuery(this)
      jQuery(this).keyup((e) -> instance._inputKeyUp(obj, e))
    )
    lastInput = inputs.filter(":last")
    lastInput.parent().addClass("input-group")
    lastInput.dynamicInputs(inputOffset, @_inputKeyUp, instance)
    return


  # _deleteButton :: () -> BootstrapDialogButton
  _deleteButton: =>
    jQuery.extend(super,
      action: (dialog) =>
        (new root.ConfirmationDialog(@_data))
          .on('confirm', =>
            @fireEvents('delete', @_dialogInfo.questionId, @_dialogInfo.type)
            dialog.close()
          )
    )


  # _prepareFilledDialog :: DOMNode -> undefined
  _prepareFilledDialog: (dialogBody) =>
    @_dialog.setTitle(@_data.utils.text.title)
    jQuery(".form-group:eq(0) input", dialogBody).val(@_dialogInfo.name)
    jQuery(".form-group:eq(1) input", dialogBody).val(@_dialogInfo.question)
    activeLabel = if @_dialogInfo.singleAnswer then 0 else 1
    jQuery(".form-group:eq(2) .btn-group label:eq(#{activeLabel})", dialogBody).addClass("active")
    for option in @_dialogInfo.options
      jQuery(".form-group:last-child > div input:last", dialogBody)
        .val(option.text).keyup()

    if @options.readonly
      jQuery("input", dialogBody).prop("readonly", true)
      # remove last "add answer" entry
      jQuery("input", dialogBody).last().parents('.input-group').remove()
      jQuery("label.btn:not(.active)", dialogBody).remove()
      @_dialog.getButton('saveButtonDialog').hide()
    return


  # _inputKeyUp :: (jQueryObject, Event) -> undefined
  _inputKeyUp: (obj, e) =>
    text = obj.val()
    error = obj.parent().next()
    if text.length is 0
      @_showInputError(error, @_getEmptyInputError())
    else
      error.html("")
    return


  # _validateForm :: () -> Boolean
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
      instance._showInputError(error, @_data.utils.text.needMultipleAnswerError)
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

  ###
  # type MultipleChoiceQuestionData = {
  #   name         :: String,
  #   question     :: String,
  #   options      :: [String],
  #   singleAnswer :: Boolean,
  #   type         :: Int
  # }
  ###
  # extractData :: () -> MultipleChoiceQuestionData
  extractData: ->
    name = jQuery("#dialog .form-group:eq(0) input").val()
    question = jQuery("#dialog .form-group:eq(1) input").val()
    singleAnswer = jQuery("#dialog input[type=radio]").first().parent().hasClass("active")
    options = []
    jQuery("#dialog .form-group:last-child .input-group input:not(:last)").each( ->
      options.push jQuery(this).val()
    )
    data =
      name: name
      question: question
      options: options
      singleAnswer: singleAnswer
      type: @_questionType
    data
