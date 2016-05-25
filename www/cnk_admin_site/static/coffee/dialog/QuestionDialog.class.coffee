root = exports ? this
root.QuestionDialog = class QuestionDialog extends root.View
  # ======== ABSTRACT FUNCTIONS ========
  # _prepareFilledDialog :: DOMNode -> undefined
  # extractData          :: () -> DialogData
  # _deleteButton        :: () -> BootstrapDialogButton
  # ====================================
  ###
  # DialogData =
  #   ActionData
  # | ExhibitData
  # | MultipleChoiceQuestionData
  # | SimpleQuestionData
  # | SortQuestionData
  # | CloneExperimentData
  ###
  # options = {
  #   Readonly :: Boolean
  # , Deletable :: Boolean
  # }
  # constructor :: (String, JsObject) -> Context
  constructor: (@_dialogData, @options = {}) ->
    super()
    @_setDefaultOptions()
    @_data = @_dialogData.data
    @_dialog = new BootstrapDialog(
      message: @_dialogData.html
      title: @_data.utils.text.titleNew
      closable: false
      buttons:
        [ @_deleteButton()
        , @_closeButton()
        , @_saveButton()
        ]
    )
    @_dialog.realize()
    @_prepareDialog(@_dialog.getModalBody())


  # _setDefaultOptions :: () -> undefined
  _setDefaultOptions: =>
    @options.readonly ?= false
    @options.deletable ?= false
    return


  # bindData :: listElement -> Context
  bindData: (@_dialogInfo) =>
    @_dialog.setMessage(@_dialogData.html)
    @_dialog.realize()
    dialogBody = @_dialog.getModalBody()
    @_prepareDialog(dialogBody)
    @_prepareFilledDialog(dialogBody)
    @


  # show :: () -> Context
  show: =>
    @_dialog.open()
    @


  # _prepareDialog :: DOMNode -> undefined
  _prepareDialog: (dialogBody) =>
    instance = this
    jQuery("input[type=text]", dialogBody).blur( ->
      obj = jQuery(this)
      error = obj.parent().next()
      obj.val(jQuery.trim(obj.val()))
      if not obj.val().length
        instance._showInputError(error, instance._getEmptyInputError())
      )
    jQuery("input.form-control", dialogBody).keypress((e) =>
      if e.which is 13
        e.preventDefault()
        jQuery(this).one('blur', (blurEvent) =>
          @_dialog.getButton('saveButtonDialog').click()
          blurEvent.stopImmediatePropagation()
        )
        jQuery(this).blur()
    )
    if not @options.deletable
      @_dialog.getButton('deleteButtonDialog').hide()
    return


  # _showInputError :: (jQueryObject, String) -> undefined
  _showInputError: (obj, message) ->
    obj.html message
    return


  # _getEmptyInputError :: () -> String
  _getEmptyInputError: =>
    @_data.utils.text.emptyInputError


  # _getInputError :: () -> String
  _getInputError: =>
    @_data.utils.text.inputError


  # _closeButton :: () -> BootstrapDialogButton
  _closeButton: =>
    id: 'cancelButtonDialog'
    label: @_data.utils.text.cancelButton
    action: (dialog) ->
      dialog.close()


  # _saveButton :: () -> BootstrapDialogButton
  _saveButton: =>
    id: 'saveButtonDialog'
    label: @_data.utils.text.saveButton
    action: (dialog) =>
      dialog.showNameDuplicatedError = @_showNameDuplicatedError
      if @_validateForm()
        @fireEvents('save', @extractData(), dialog)


  # _deleteButton :: () -> BootstrapDialogButton
  _deleteButton: =>
    id: 'deleteButtonDialog'
    label: @_data.utils.text.deleteButton
    cssClass: 'btn-danger delete-button'
    action: (dialog) ->
      dialog.close()


  # _showNameDuplicatedError :: () -> undefined
  _showNameDuplicatedError: =>
    jQuery('#dialog .form-group:first-child div:last-child').html(@_data.utils.text.nameDuplicatedError)
    return


  # _validateForm :: () -> Boolean
  _validateForm: =>
    isValid = true
    instance = this
    jQuery "#dialog input[type=text]"
      .each( ->
        obj = jQuery(this)
        text = obj.val()
        if text.length is 0
          isValid = false
          error = obj.parent().next()
          instance._showInputError(error, instance._data.utils.text.emptyInputError)
      )
    isValid
