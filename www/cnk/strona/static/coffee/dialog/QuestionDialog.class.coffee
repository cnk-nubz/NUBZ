root = exports ? this
root.QuestionDialog = class QuestionDialog
  readonly: false
  constructor: (@_url, @_saveHandler = (->)) ->
    jQuery.getJSON(@_url, null, (data) =>
      @_data = data.data
      @_dialogHTML = data.html
      @_dialog = new BootstrapDialog(
        message: data.html
        title: data.data.utils.text.title
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

  _prepareDialog: (dialog) =>
    instance = this
    jQuery("input[type=text]", dialog).blur( ->
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
      if @_validateForm()
        @_saveHandler(@extractData())
        dialog.close()

  _validateForm: =>
    isValid = true
    instance = this
    jQuery "#dialog input[type=text]"
      .each( ->
        text = jQuery(this).val()
        regex = new RegExp(instance._data.utils.regex.input)
        if not text.match regex
          isValid = false
          if text.length
            instance._showInputError(jQuery(this).parent().next(), instance._data.utils.text.inputError)
          else
            instance._showInputError(jQuery(this).parent().next(), instance._data.utils.text.emptyInputError)
      )
    isValid
