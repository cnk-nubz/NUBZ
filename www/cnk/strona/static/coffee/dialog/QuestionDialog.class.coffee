root = exports ? this
root.QuestionDialog = class QuestionDialog
  # ======== ABSTRACT FUNCTIONS ========
  # _prepareFilledDialog :: DOMNode -> undefined
  # extractData          :: () -> DialogData
  # ====================================
  ###
  # DialogData =
  #   ActionData
  # | ExhibitData
  # | MultipleChoiceQuestionData
  # | SimpleQuestionData
  # | SortQuestionData
  ###
  readonly: false
  # constructor :: (String, (DialogData, BootstrapDialog) -> undefined)
  constructor: (@_url, @_saveHandler = (->)) ->
    if root.cachedData.hasOwnProperty(@_url)
      @_init(root.cachedData[@_url])
    else
      jQuery.getJSON(@_url, null, (data) =>
        root.cachedData[@_url] = data
        @_init(data)
      )


  _init: (data) =>
    @_data = data.data
    @_dialogHTML = data.html
    @_dialog = new BootstrapDialog(
      message: data.html
      title: data.data.utils.text.titleNew
      closable: false
      buttons: [@_closeButton(), @_saveButton()]
    )
    @_dialog.realize()
    @_prepareDialog(@_dialog.getModalContent())


  # bindData :: listElement -> Context
  bindData: (@_dialogInfo) =>
    @_dialog.setMessage(@_dialogHTML)
    @_dialog.realize()
    dialogBody = @_dialog.getModalContent()
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
        @_saveHandler(@extractData(), dialog)


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
