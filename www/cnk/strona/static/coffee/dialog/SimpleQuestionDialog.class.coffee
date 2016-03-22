root = exports ? this
root.SimpleQuestionDialog = class SimpleQuestionDialog extends root.QuestionDialog
  _prepareDialog: (dialog) =>
    super
    radioGroup = @_data.data[2][1].radioGroup
    instance = this
    if not @_dialogInfo?.type?
      jQuery("label.#{radioGroup}", dialog).filter(":first").addClass("active")

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
    jQuery(".form-group:eq(0) input", dialog).val(@_dialogInfo.name)
    jQuery(".form-group:eq(1) input", dialog).val(@_dialogInfo.question)
    jQuery(".form-group:eq(2) .btn-group label:eq(#{@_dialogInfo.answerType})", dialog).addClass("active")
    if @readonly
      jQuery("#dialog input", dialog).prop("readonly", true)
      jQuery("#dialog .btn:not(.active)", dialog).remove()
      @_dialog.getButton('saveButtonDialog').hide()
    @

  extractData: =>
    answerAsNumberInput = jQuery("#dialog input[type=radio]").first()
    answerAsNumberLabel = answerAsNumberInput.parent()
    name = jQuery("#dialog .form-group:eq(0) input").val()
    question = jQuery("#dialog .form-group:eq(1) input").val()
    answerAsNumber = not answerAsNumberLabel.hasClass("active")
    data =
      name: name
      question: question
      answerAsNumber: answerAsNumber
    data
