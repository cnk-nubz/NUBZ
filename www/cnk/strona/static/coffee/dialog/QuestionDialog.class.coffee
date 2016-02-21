root = exports ? this
root.QuestionDialog = class QuestionDialog
  readonly: false
  constructor: (@_url, @_saveHandler = (->)) ->
    @_getDialogMessage()

  _getDialogMessage: =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      url: @_url
      context: this
      success: (data) ->
        @_data = data.data
        @_dialog = new BootstrapDialog(
          message: data.html
          title: data.data.utils.text.title
          onshown: @_dialogCreated
          closable: false
          buttons: [@_closeButton(), @_saveButton()]
        )
    )

  bindData: (@_dialogInfo) => @

  show: =>
    @_dialog.open()

  _dialogCreated: =>
    instance = this
    jQuery("#dialog input[type=text]").blur( ->
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
    label: @_data.utils.text.cancelButton
    action: (dialog) ->
      dialog.close()

  _saveButton: =>
    label: @_data.utils.text.saveButton
    action: (dialog) =>
      if @_validateForm()
        @_saveHandler()
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
