root = exports ? this
root.ExhibitDialog = class ExhibitDialog extends root.QuestionDialog
  constructor: (url, saveHandler, @_canvasActiveFloor) ->
    super(url, saveHandler)
  # _prepareDialog :: DOMNode -> undefined
  _prepareDialog: (dialogBody) =>
    super
    @_dialog.setTitle(@_data.utils.text.title)
    jQuery(".form-group:eq(1) .btn-group .floorNum:eq(#{@_canvasActiveFloor})", dialogBody)
      .addClass("active")
    jQuery(".popoverButton", dialogBody).css("background-color": "#9DE35A")
    instance = this
    jQuery("input[type=text]", dialogBody)
      .each( ->
        obj = jQuery(this)
        jQuery(this).keyup((e) -> instance._inputKeyUp(obj, e))
      )
    jQuery(".popoverButton", dialogBody)
      .each( ->
        obj = jQuery(this)
        error = obj.parent().next()
      )
    jQuery.getJSON('getHTML?name=colorPickerPopover', (data) ->
      jQuery(dialogBody).parents(".modal").click( ->
        jQuery('.popoverButton', dialogBody).popover('hide')
      )
      jQuery('.popoverButton', dialogBody).attr(
        'data-content': data.html
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
    )
    return


  # _prepareFilledDialog :: DOMNode -> undefined
  _prepareFilledDialog: (dialogBody) =>
    @_dialog.setTitle(@_data.utils.text.editTitle)
    jQuery(".form-group:eq(0) input", dialogBody).val(@_dialogInfo.name)
    jQuery(".form-group .btn-group .floorNum", dialogBody).removeClass("active")
    jQuery(".form-group:eq(1) .btn-group .floorNum:eq(#{@_dialogInfo.floor})", dialogBody).addClass("active")
    jQuery(".popoverButton", dialogBody).css("background-color": @_dialogInfo.color)
    Pdialog = dialogBody.parent()
    jQuery(".bootstrap-dialog-footer-buttons", Pdialog).prepend(@_data.utils.text.deleteButtonHtml)
    jQuery(".delete-button", Pdialog).click( =>
      new root.ConfirmDialog('getHTML?name=confirmExhibitDelDialog', @_deleteExhibit)
    )
    jQuery("input", dialogBody).prop("readonly", true)
    if @readonly is true
      jQuery("label.floorNum.btn:not(.active)", dialogBody).remove()
      jQuery(".popoverButton", dialogBody).prop("disabled", true)
    return


  # setDeleteHandler :: (->) -> Context
  setDeleteHandler: (@_successfullDeleteHandler) => @


  # _deleteExhibit :: () -> undefined
  _deleteExhibit: =>
    toSend =
      jsonData:
        JSON.stringify(
          id: @_dialogInfo.id
        )
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      dataType: 'json'
      url: '/deleteExhibit/'
      data: toSend
      success: =>
        if @_successfullDeleteHandler
          @_successfullDeleteHandler(@_dialogInfo.id)
        @_dialog.close()
    )
    return


  # _inputKeyUp :: (jQueryObject, Event) -> undefined
  _inputKeyUp: (obj, e) =>
    text = obj.val()
    error = obj.parent().next()
    if text.length is 0
      @_showInputError(error, @_getEmptyInputError())
    else
      error.html("")
    return


  # _saveButton :: () -> BootstrapDialogButton
  _saveButton: =>
    label: super.label
    action: (dialog) =>
      if @readonly is false
        super.action(dialog)
      else
        dialog.close()


  ###
  # type ExhibitData = {
  #   id     :: Int,
  #   name   :: String,
  #   rgbHex :: Int,
  #   floor  :: Int
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
      id: parseInt(@_dialogInfo?.id)
      name: editedName
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
