root = exports ? this
root.ExhibitDialog = class ExhibitDialog extends root.QuestionDialog
  _dialogCreated: =>
    super
    if @_dialogInfo?
      @_dialog.setTitle(@_data.utils.text['editTitle'])
      if not @_dialogInfo.floor?
        @_dialogInfo.floor = 2
      jQuery("#dialog .form-group:eq(0) input").val(@_dialogInfo.name)
      jQuery("#dialog .form-group:eq(1) .btn-group .floorNum:eq(#{@_dialogInfo.floor})").addClass("active")
      if not @_dialogInfo.color?
        jQuery("#dialog .form-group:eq(2)").hide()
        jQuery("#dialog .popoverButton").css("background-color": "#9DE35A")
      else
        jQuery("#dialog .popoverButton").css("background-color": @_dialogInfo.color)
    else
      @mapData = new MapDataHandler()
      jQuery("#dialog .form-group:eq(1) .btn-group .floorNum:eq(#{@mapData.activeFloor})").addClass("active")
      jQuery("#dialog .popoverButton").css("background-color": "#9DE35A")

    instance = this
    jQuery "#dialog input[type=text]"
      .each( ->
        obj = jQuery(this)
        error = obj.parent().next()
        error.css("color", instance._data.utils.style.inputErrorColor)
        regex = new RegExp(instance._data.utils.regex.input)
        jQuery(this).keyup((e) -> instance._inputKeyUp(regex)(obj, e))
      )
    jQuery "#dialog .popoverButton"
      .each( ->
        obj = jQuery(this)
        error = obj.parent().next()
        error.css("color", instance._data.utils.style.inputErrorColor)
      )

    jQuery "#dialog .form-group:eq(1) .btn-group .floorNum:eq(0)"
      .click( ->
        jQuery("#dialog .form-group:eq(2)").show()
      )

    jQuery "#dialog .form-group:eq(1) .btn-group .floorNum:eq(1)"
      .click( ->
        jQuery("#dialog .form-group:eq(2)").show()
      )

    jQuery "#dialog .form-group:eq(1) .btn-group .floorNum:eq(2)"
      .click( ->
        jQuery("#dialog .form-group:eq(2)").hide()
      )

    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      context: this
      url: '/getColorPickerPopoverContent/'
      success: (data) =>
        jQuery('.popoverButton').attr(
          'data-content': data
        ).popover().on('shown.bs.popover', ( ->
          jQuery("div.popover").css("z-index", 5000)
          jQuery('div.popover button').click( ->
            rgbvals = jQuery(this).css("background-color")
            hexval = instance._rgb2hex(rgbvals).toUpperCase()
            jQuery('.popoverButton').css("background-color", hexval)
            instance._colorPopoverClose()
          )
        ))
    )

    if @readonly is true
      jQuery("#dialog input").prop("readonly", true)
      jQuery("#dialog label.floorNum.btn:not(.active)").remove()
      jQuery("#dialog .popoverButton").prop("disabled", true)

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
      @_colorPopoverClose()
      super.action(dialog)

  _saveButton: =>
    label: super.label
    action: (dialog) =>
      if @readonly is false
        @_colorPopoverClose()
        super.action(dialog)
      else
        dialog.close()

  _colorPopoverClose: =>
    jQuery('.popoverButton').popover('hide')

  extractData: =>
    editedName = jQuery("#dialog input[type=text]").val()
    floorText = jQuery("#dialog .floorNum.active").text()
    if jQuery.trim(floorText) is "brak"
      floor = null
    else
      floor = +floorText
    exhibitColor = @_rgb2hex(jQuery(".popoverButton").css("background-color"))[1..]
    changedData =
      name: editedName
      floor: floor
      color: exhibitColor
    changedData

  _validateForm: =>
    isValid = true
    instance = this
    jQuery "#dialog input[type=text]"
      .each( ->
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
    isValid

  _rgb2hex: (rgb) ->
    hex = (x) -> "0#{parseInt(x).toString(16)}"[-2..]
    if /^#[0-9A-F]{6}$/i.test(rgb)
      return rgb
    rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/)
    '#' + hex(rgb[1]) + hex(rgb[2]) + hex(rgb[3])
