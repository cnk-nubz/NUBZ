root = exports ? this
root.SimpleQuestionDialog = class SimpleQuestionDialog extends root.QuestionDialog
  # _prepareDialog :: DOMNode -> undefined
  _prepareDialog: (dialogBody) =>
    super
    radioGroup = @_data.data[2][1].radioGroup
    instance = this
    if not @_dialogInfo?.type?
      jQuery("label.#{radioGroup}", dialogBody).filter(":first").addClass("active")

    jQuery("input[type=text]", dialogBody)
      .each( ->
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


  # _prepareFilledDialog :: DOMNode -> undefined
  _prepareFilledDialog: (dialogBody) =>
    @_dialog.setTitle(@_data.utils.text.title)
    jQuery(".form-group:eq(0) input", dialogBody).val(@_dialogInfo.name)
    jQuery(".form-group:eq(1) input", dialogBody).val(@_dialogInfo.question)
    jQuery(".form-group:eq(2) .btn-group label:eq(#{@_dialogInfo.answerType})", dialogBody).addClass("active")
    if @readonly
      jQuery("#dialog input", dialogBody).prop("readonly", true)
      jQuery("#dialog .btn:not(.active)", dialogBody).remove()
      @_dialog.getButton('saveButtonDialog').hide()
    return

  ###
  # type SimpleQuestionData = {
  #   name           :: String,
  #   question       :: String,
  #   answerAsNumber :: Boolean
  # }
  ###
  # extractData :: () -> SimpleQuestionData
  extractData: ->
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
