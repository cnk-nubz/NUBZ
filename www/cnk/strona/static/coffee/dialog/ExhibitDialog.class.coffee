root = exports ? this
root.ExhibitDialog = class ExhibitDialog extends root.QuestionDialog
  _dialogCreated: =>
    super
    if @_dialogInfo?
      if @_dialogInfo.floor == null
        @_dialogInfo.floor = "brak"
      jQuery("#dialog .form-group:eq(0) input").val(@_dialogInfo.name)
      jQuery("#dialog .form-group:eq(1) .btn-group label:contains(#{@_dialogInfo.floor})").addClass("active")
      jQuery("#dialog #popover_button").css("background-color": "#" + @_dialogInfo.color)

    radioGroup = @_data.data[1][1].radioGroup
    jQuery("#dialog label.#{radioGroup}").filter(":first").addClass("active") unless @_dialogInfo?

    instance = this
    jQuery "#dialog input[type=text]"
      .each( ->
        obj = jQuery(this)
        error = obj.parent().next()
        error.css("color", instance._data.utils.style.inputErrorColor)
        regex = new RegExp(instance._data.utils.regex.input)
        jQuery(this).keyup((e) -> instance._inputKeyUp(regex)(obj, e))
      )
    jQuery "#dialog #popover_button"
      .each( ->
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
            instance._colorPopoverClose()
            colorError = jQuery("#popover_button").parent().next()
            instance._showInputError(colorError, "")
          )
        ))
    )

    if @readonly == true
      console.log "asdf"
      jQuery("#dialog input").prop("readonly", true)
      jQuery("#dialog label.floorNum.btn:not(.active)").remove()
      jQuery("#dialog #popover_button").prop("disabled", true)

  _inputKeyUp: (regex) =>
      (obj, e) =>
        error = obj.parent().next()
        if not obj.val().match regex
          if obj.val().length
            @_showInputError(error, @_getInputError())
          else
            @_showInputError(error, @_getEmptyInputError())
        else
          error.html("")
        return

  _closeButton: =>
    label: super.label
    action: (dialog) =>
      super.action(dialog)
      @_colorPopoverClose()

  _saveButton: =>
    label: super.label
    action: (dialog) =>
      if @readonly == false
        super.action(dialog)
        @_colorPopoverClose()
      else
        dialog.close()

  _colorPopoverClose: =>
    jQuery('#popover_button').popover('hide')

  extractData: =>
    editedName = jQuery("#dialog input[type=text]").val()
    floorVal = jQuery("#dialog .floorNum.active").text()
    if floorVal.match "brak"
      floorVal = null
    else
      floorVal = parseInt(floorVal)
    exhibitColor = _rgb2hex(jQuery("#popover_button").css("background-color")).substring(1)
    changedData =
      name: editedName
      floor: floorVal
      color: exhibitColor
    changedData

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
            instance._showInputError(error, instance._getInputError())
          else
            instance._showInputError(error, instance._getEmptyInputError())
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
