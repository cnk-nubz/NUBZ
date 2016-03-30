root = exports ? this
root.QuestionDialog = class QuestionDialog
  readonly: false
  constructor: (@_url, @_saveHandler = (->)) ->
    jQuery.getJSON(@_url, null, (data) =>
      @_data = data.data
      @_dialogHTML = data.html
      @_dialog = new BootstrapDialog(
        message: data.html
        title: data.data.utils.text.titleNew
        closable: false
        buttons: [@_closeButton(), @_saveButton()]
      )
      @_dialog.realize()
      @_prepareDialog(@_dialog.getModalBody())
    )

  bindData: (@_dialogInfo) =>
    @_dialog.setMessage(@_dialogHTML)
    @_dialog.realize()
    dialogBody = @_dialog.getModalBody()
    @_prepareDialog(dialogBody)
    @_prepareFilledDialog(dialogBody)
    @

  show: =>
    @_dialog.open()

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

  _showInputError: (obj, message) =>
    obj.html message
    return

  _getEmptyInputError: =>
    @_data.utils.text.emptyInputError

  _getInputError: =>
    @_data.utils.text.inputError

  _closeButton: =>
    id: 'closeButtonDialog'
    label: @_data.utils.text.cancelButton
    action: (dialog) ->
      dialog.close()

  _saveButton: =>
    id: 'saveButtonDialog'
    label: @_data.utils.text.saveButton
    action: (dialog) =>
      dialog.showNameDuplicatedError = @_showNameDuplicateError
      if @_validateForm()
        @_saveHandler(@extractData(), dialog)
        dialog.close()

  _showNameDuplicatedError: =>
    jQuery('#dialog .form-group:first-child div:last-child').html(@_data.utils.text.nameDuplicatedError)
    return

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
