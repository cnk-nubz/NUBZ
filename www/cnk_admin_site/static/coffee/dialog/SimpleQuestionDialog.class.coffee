root = exports ? this
root.SimpleQuestionDialog = class SimpleQuestionDialog extends root.QuestionDialog
  constructor: (dialogData, options = {}) ->
    super(dialogData, options)
    @_questionType = 0
  # _prepareDialog :: DOMNode -> undefined
  _prepareDialog: (dialogBody) =>
    super
    radioGroup = @_data.data[2][1].radioGroup
    instance = this
    if not @_dialogInfo?.type?
      jQuery("label.#{radioGroup}", dialogBody).filter(":first").addClass("active")
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
    jQuery(".form-group:eq(2) .btn-group label:eq(#{@_dialogInfo.answerType})", dialogBody).addClass("active")
    if @options.readonly
      jQuery("#dialog input", dialogBody).prop("readonly", true)
      jQuery("#dialog .btn:not(.active)", dialogBody).remove()
      @_dialog.getButton('saveButtonDialog').hide()
    return

  ###
  # type SimpleQuestionData = {
  #   name           :: String,
  #   question       :: String,
  #   answerType     :: String,
  #   type           :: Int
  # }
  ###
  # extractData :: () -> SimpleQuestionData
  extractData: ->
    answerAsTextInput = jQuery("#dialog input[type=radio]").first()
    answerAsTextLabel = answerAsTextInput.parent()
    name = jQuery("#dialog .form-group:eq(0) input").val()
    question = jQuery("#dialog .form-group:eq(1) input").val()
    if answerAsTextLabel.hasClass("active")
      answerType = 'TEXT'
    else
      answerType = 'NUMBER'
    data =
      name: name
      question: question
      answerType: answerType
      type: @_questionType
    data
