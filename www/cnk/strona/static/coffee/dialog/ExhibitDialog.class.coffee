root = exports ? this
root.ExhibitDialog = class ExhibitDialog extends root.QuestionDialog
  _dialogCreated: =>
    super
    console.log @_data
    radioGroup = @_data.data[1][1].radioGroup
    jQuery "#dialog label.#{radioGroup}"
      .filter ":first"
      .addClass "active"

    instance = this
    jQuery "#dialog input[type=text]"
      .each(->
        obj = jQuery(this)
        error = obj.parent().next()
        error.css("color", instance._data.utils.style.inputErrorColor)
        regex = new RegExp(instance._data.utils.regex.input)
        jQuery(this).keyup((e) -> instance._inputKeyUp(regex)(obj, e))
      )
    jQuery "#dialog #popover_button"
      .each(->
        obj = jQuery(this)
        error = obj.parent().next()
        error.css("color", instance._data.utils.style.inputErrorColor)
      )

    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      context: this
      url: '/getColorPickerPopoverContent/'
      success: (data) =>
        jQuery('#popover_button').attr(
          'data-content': data
        ).popover().on('shown.bs.popover', (->
          $('.colorpick').click(->
            rgbvals = $(this).css("background-color")
            hexval = _rgb2hex(rgbvals).toUpperCase()
            jQuery('#popover_button').css("background-color", hexval)
            jQuery('#popover_button').popover('hide')
            colorError = jQuery("#popover_button").parent().next()
            instance._showInputError(colorError, "")
          )
        ))
    )

  _inputKeyUp: (regex) =>
      (obj, e) =>
        error = obj.parent().next()
        if not obj.val().match regex
          if obj.val().length
            @_showInputError(error, @_data.utils.text.inputError)
          else
            @_showInputError(error, @_data.utils.text.emptyInputError)
        else
          error.html("")
        return

  _closeButton: =>
    label: @_data.utils.text.cancelButton
    action: (dialog) ->
      jQuery('#popover_button').popover('hide')
      dialog.close()

  _saveButton: =>
    label: @_data.utils.text.saveButton
    action: (dialog) =>
      jQuery('#popover_button').popover('hide')
      if @_validateForm()
        @_sendDataToHandler()
        dialog.close()

  _sendDataToHandler: =>
    editedName = jQuery("#dialog input[type=text]").val()
    floorVal = jQuery("#dialog .btn-group .active").val()
    if floorVal == "brak"
      floorVal = null
    else
      floorVal = parseInt(floorVal)
    exhibitColor = _rgb2hex(jQuery("#popover_button").css("background-color")).substring(1)
    changedData =
      name: editedName
      floor: floorVal
      color: exhibitColor
    # TODO: fire event

  _validateForm: =>
    isValid = true
    instance = this
    jQuery "#dialog input[type=text]"
      .each(->
        text = jQuery(this).val()
        regex = new RegExp(instance._data.utils.regex.input)
        if not text.match regex
          isValid = false
          error = jQuery(this).parent().next()
          if text.length
            instance._showInputError(error, instance._data.utils.text.inputError)
          else
            instance._showInputError(error, instance._data.utils.text.emptyInputError)
      )
    colorError = jQuery("#popover_button").parent().next()
    if _rgb2hex(jQuery("#popover_button").css('background-color')) != '#ffffff'
      @_showInputError(colorError, "")
    else
      isValid = false
      @_showInputError(colorError, @_data.utils.text.colorError)
    isValid

  _rgb2hex = (rgb) ->
    hex = (x) ->
      ('0' + parseInt(x).toString(16)).slice -2
    if /^#[0-9A-F]{6}$/i.test(rgb)
      return rgb
    rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/)
    '#' + hex(rgb[1]) + hex(rgb[2]) + hex(rgb[3])
