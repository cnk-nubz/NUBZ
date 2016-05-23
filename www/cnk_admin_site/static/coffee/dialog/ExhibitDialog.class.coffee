root = exports ? this
root.ExhibitDialog = class ExhibitDialog extends root.QuestionDialog
  constructor: (dialogData, options = {}, @_canvasActiveFloor) ->
    super(dialogData, options)
  # _prepareDialog :: DOMNode -> undefined
  _prepareDialog: (dialogBody) =>
    super
    @_dialog.setTitle(@_data.utils.text.title)
    jQuery(".form-group:eq(1) .btn-group .floorNum:eq(#{@_canvasActiveFloor})", dialogBody)
      .addClass("active")
    jQuery(".popoverButton", dialogBody)
      .css("background-color", @_data.utils.css.defaultColor)
    instance = this

    jQuery(".popoverButton", dialogBody)
      .each( ->
        obj = jQuery(this)
        error = obj.parent().next()
      )

    jQuery(dialogBody).parents(".modal").click( ->
      jQuery('.popoverButton', dialogBody).popover('hide')
    )

    jQuery('.popoverButton', dialogBody).click((event) ->
      if jQuery('.popover').is(':visible')
        jQuery(this).popover('hide')
      else
        jQuery(this).popover('show')
      event.stopPropagation()
    )

    jQuery('.popoverButton', dialogBody).popover().on('shown.bs.popover', ( ->
      jQuery("div.popover button").mousedown( ->
        rgbvals = jQuery(this).css("background-color")
        hexval = instance._rgb2hex(rgbvals)
        jQuery('.popoverButton').css("background-color", hexval)
        jQuery('.popoverButton', dialogBody).popover('hide')
      )
    ))
    return


  # _prepareFilledDialog :: DOMNode -> undefined
  _prepareFilledDialog: (dialogBody) =>
    @_dialog.setTitle(@_data.utils.text.editTitle)
    jQuery(".form-group:eq(0) input", dialogBody).val(@_dialogInfo.name)
    jQuery(".form-group .btn-group .floorNum", dialogBody).removeClass("active")
    jQuery(".form-group:eq(1) .btn-group .floorNum:eq(#{@_dialogInfo.floor})", dialogBody).addClass("active")
    jQuery(".popoverButton", dialogBody).css("background-color": @_dialogInfo.color)
    jQuery("input", dialogBody).prop("readonly", true)
    if @options.readonly is true
      jQuery("label.floorNum.btn:not(.active)", dialogBody).remove()
      jQuery(".popoverButton", dialogBody).prop("disabled", true)
    return


  # _inputKeyUp :: (jQueryObject, Event) -> undefined
  _inputKeyUp: (obj, e) =>
    text = obj.val()
    error = obj.parent().next()
    if text.length is 0
      @_showInputError(error, @_getEmptyInputError())
    else
      error.html('')
    return


  # _deleteButton :: () -> BootstrapDialogButton
  _deleteButton: =>
    jQuery.extend(super,
      action: (dialog) =>
        (new root.ConfirmationDialog(@_data))
          .on('confirm', =>
            @fireEvents('delete', @_dialogInfo.exhibitId)
            dialog.close()
          )
    )


  ###
  # type ExhibitData = {
  #   exhibitId :: Int,
  #   name      :: String,
  #   rgbHex    :: Int,
  #   floor     :: Int
  # }
  ###
  # extractData :: () -> ExhibitData
  extractData: =>
    editedName = jQuery("#dialog input[type=text]").val()
    floorText = jQuery("#dialog .floorNum.active").text()
    if jQuery.trim(floorText) is "brak"
      floor = null
    else
      floor = +floorText
    exhibitColor = parseInt(@_rgb2hex(jQuery(".popoverButton").css("background-color"))[1..], 16)
    changedData =
      exhibitId: parseInt(@_dialogInfo?.exhibitId)
      name: jQuery("<div>").text(editedName).html() # escape name
      rgbHex: exhibitColor
      floor: floor
    changedData


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
          instance._showInputError(error, instance._getEmptyInputError())
      )
    isValid


  # _rgb2hex :: String -> String
  _rgb2hex: (rgb) ->
    hex = (x) -> "0#{parseInt(x).toString(16)}"[-2..]
    if /^#[0-9A-F]{6}$/i.test(rgb)
      return rgb
    rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/)
    '#' + hex(rgb[1]) + hex(rgb[2]) + hex(rgb[3])
