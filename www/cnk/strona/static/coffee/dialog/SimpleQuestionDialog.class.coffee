root = exports ? this
root.SimpleQuestionDialog = class SimpleQuestionDialog extends root.QuestionDialog
  _dialogCreated: =>
    super
    if @_dialogInfo?
      jQuery("#dialog .form-group:eq(0) input").val(@_dialogInfo.name)
      jQuery("#dialog .form-group:eq(1) input").val(@_dialogInfo.question)
      jQuery("#dialog .form-group:eq(2) .btn-group label:eq(#{@_dialogInfo.answerType})").addClass("active")
    radioGroup = @_data.data[2][1].radioGroup
    instance = this
    jQuery("#dialog label.#{radioGroup}").filter(":first").addClass("active") unless @_dialogInfo?.type?

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

    if @readonly
      jQuery("#dialog input").prop("readonly", true)
      jQuery("#dialog .btn:not(.active)").remove()

    return

  extractData: =>
    name = jQuery("#dialog .form-group:eq(0) input").val()
    question = jQuery("#dialog .form-group:eq(1) input").val()
    answerAsNumber = not jQuery("#dialog input[type=radio]").first().parent().hasClass("active")
    data =
      name: name
      question: question
      answerAsNumber: answerAsNumber
    data
