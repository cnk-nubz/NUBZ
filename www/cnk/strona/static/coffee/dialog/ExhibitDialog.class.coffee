root = exports ? this
root.ExhibitDialog = class ExhibitDialog extends root.QuestionDialog
  _prepareDialog: (dialog) =>
    super
    @mapData = new MapDataHandler()
    jQuery(".form-group:eq(1) .btn-group .floorNum:eq(#{@mapData.activeFloor})", dialog).addClass("active")
    jQuery(".popoverButton", dialog).css("background-color": "#9DE35A")
    instance = this
    jQuery("input[type=text]", dialog)
      .each( ->
        obj = jQuery(this)
        error = obj.parent().next()
        error.css("color", instance._data.utils.style.inputErrorColor)
        regex = new RegExp(instance._data.utils.regex.input)
        jQuery(this).keyup((e) -> instance._inputKeyUp(regex)(obj, e))
      )
    jQuery(".popoverButton", dialog)
      .each( ->
        obj = jQuery(this)
        error = obj.parent().next()
        error.css("color", instance._data.utils.style.inputErrorColor)
      )
    @_popoverOpened = false
    jQuery.getJSON('getHTML?name=colorPickerPopover', (data) =>
      jQuery(dialog).parents(".modal").click( ->
        jQuery('.popoverButton', dialog).popover('hide')
      )
      jQuery('.popoverButton', dialog).attr(
        'data-content': data.html
      )
      jQuery('.popoverButton', dialog).click((event) ->
        if jQuery('.popover').is(':visible')
          jQuery(this).popover('hide')
        else
          jQuery(this).popover('show')
        event.stopPropagation()
      )
      jQuery('.popoverButton', dialog).popover().on('shown.bs.popover', ( ->
        jQuery("div.popover button").mousedown( ->
          rgbvals = jQuery(this).css("background-color")
          hexval = instance._rgb2hex(rgbvals)
          jQuery('.popoverButton').css("background-color", hexval)
          jQuery('.popoverButton', dialog).popover('hide')
        )
      ))
    )

  _prepareFilledDialog: (dialog) =>
    @_dialog.setTitle(@_data.utils.text.editTitle)
    if not @_dialogInfo.floor?
      @_dialogInfo.floor = 2
    jQuery(".form-group:eq(0) input", dialog).val(@_dialogInfo.name)
    jQuery(".form-group .btn-group .floorNum", dialog).removeClass("active")
    jQuery(".form-group:eq(1) .btn-group .floorNum:eq(#{@_dialogInfo.floor})", dialog).addClass("active")
    jQuery(".popoverButton", dialog).css("background-color": @_dialogInfo.color)
    jQuery("input", dialog).prop("readonly", true)
    if @readonly is true
      jQuery("label.floorNum.btn:not(.active)", dialog).remove()
      jQuery(".popoverButton", dialog).prop("disabled", true)

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

  _saveButton: =>
    label: super.label
    action: (dialog) =>
      if @readonly is false
        super.action(dialog)
      else
        dialog.close()

  extractData: =>
    editedName = jQuery("#dialog input[type=text]").val()
    floorText = jQuery("#dialog .floorNum.active").text()
    if jQuery.trim(floorText) is "brak"
      floor = null
    else
      floor = +floorText
    exhibitColor = parseInt(@_rgb2hex(jQuery(".popoverButton").css("background-color"))[1..], 16)
    changedData =
      id: @_dialogInfo?.id
      name: editedName
      rgbHex: exhibitColor
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
