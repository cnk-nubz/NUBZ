root = exports ? this
root.SimpleQuestionDialog = class SimpleQuestionDialog extends root.QuestionDialog
  _prepareDialog: (dialogBody) =>
    super
    radioGroup = @_data.data[2][1].radioGroup
    instance = this
    if not @_dialogInfo?.type?
      jQuery("label.#{radioGroup}", dialogBody).filter(":first").addClass("active")

    jQuery("input[type=text]", dialogBody)
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

  _prepareFilledDialog: (dialogBody) =>
    @_dialog.setTitle(@_data.utils.text.title)
    jQuery(".form-group:eq(0) input", dialogBody).val(@_dialogInfo.name)
    jQuery(".form-group:eq(1) input", dialogBody).val(@_dialogInfo.question)
    jQuery(".form-group:eq(2) .btn-group label:eq(#{@_dialogInfo.answerType})", dialogBody).addClass("active")
    if @readonly
      jQuery("#dialog input", dialogBody).prop("readonly", true)
      jQuery("#dialog .btn:not(.active)", dialogBody).remove()
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
