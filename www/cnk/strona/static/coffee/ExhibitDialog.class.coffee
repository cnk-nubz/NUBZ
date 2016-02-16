root = exports ? this
root.ExhibitDialog = class ExhibitDialog
  nameEditable: false
  onCloseHandler: -> return
  constructor: (@_name, @_floor, @_onSaveHandler) ->

  show: =>
    _message = d3.select(document.createElement("div"))
      .append "div"
        .classed "container-fluid", true
    @_createHtml(_message, @_name, @nameEditable)

    @_dialog = new BootstrapDialog(
      title: if @_name then "Edycja eksponatu" else "Tworzenie eksponatu"
      message: _message[0][0].outerHTML
      closable: false
      buttons: [@_dialogCancel(), @_dialogSave()]
      onshown: @_dialogShown
    )
    @_dialog.realize()
    @_dialog.open()

  _createHtml: (parent, exhibitName, isNameEditable) =>
    form = @_appendHorizontalForm(parent)

    # first line
    ## structure
    nameLine = @_appendFormLine(form)
    nameLabel = @_appendFormLabel(nameLine, "Nazwa", 2)
    nameInput = @_appendFormInput(nameLine, 10)
    nameError = @_appendShiftedDiv(nameLine, 2, 10)
    ## configuration
    nameLabel.attr(for: "dialogName")
    nameError.attr(id: "dialogError").style("color": "#D8000C")
    nameInput.attr(
      id: "dialogName"
      type: "text"
      value: exhibitName
      disabled: "true" unless isNameEditable
    )

    # second line
    ## structure
    floorLine = @_appendFormLine(form)
    floorLabel = @_appendFormLabel(floorLine, "Piętro", 2)
    floorOptions = @_appendOptionsGroup(floorLine, 10)
    @_appendOption(floorOptions, "dialogOption1", "0", @_floor is 0)
    @_appendOption(floorOptions, "dialogOption2", "1", @_floor is 1)
    @_appendOption(floorOptions, "dialogOption3", "brak", not @_floor?)
    ## configuration
    floorOptions.attr(
      role: "group"
      'data-toggle': "buttons"
    )

    # third line
    ## structure
    colorLine = @_appendFormLine(form)
    colorLabel = @_appendFormLabel(colorLine, "Kolor", 2)
    colorChoose = @_appendColorButton(colorLine)
    colorError = @_appendShiftedDiv(colorLine, 2, 10)
    colorError.attr(id: "colorError").style("color": "#D8000C")

  _appendHorizontalForm: (parent) ->
    parent.append "form"
      .classed "form-horizontal", true
      .attr(
        role: "form"
      )

  _appendFormLine: (form) ->
    form.append "div"
      .classed "form-group", true

  _appendLine: (parent) ->
    parent.append "div"
      .classed "form-group", true

  _appendFormLabel: (parent, text, columns) ->
    parent.append "label"
      .classed "col-sm-#{columns} control-label", true
      .text "#{text}:"

  _appendFormInput: (parent, columns) ->
    @_appendDiv(parent, columns)
      .append "input"
        .classed "form-control", true

  _appendOptionsGroup: (parent, columns) ->
    @_appendDiv(parent, columns, "btn-group")

  _appendOption: (optionsGroup, id, text, isActive) =>
     optionsGroup.append "label"
        .attr(id: id)
        .text text
        .classed "btn btn-default #{"active" if isActive}", true
        .append "input"
          .attr(
            type: "radio"
            name: "rfloor"
          )

  _appendShiftedDiv: (parent, offset, columns, classed = "") ->
    @_appendDiv(parent, columns, "col-sm-offset-#{offset}")

  _appendDiv: (parent, columns, classed = "") ->
    parent.append "div"
     .classed "col-sm-#{columns} #{classed}", true

  _appendColorButton: (parent) ->
    popoverDiv = parent.append "div"
      .classed "col-sm-10", true
    popoverContent = @_createPopoverContent()
    @_colorsPopover = popoverDiv.append "button"
      .classed "btn btn-default", true
      .attr(
        id: "colorsPopover"
        type: "button"
        'data-original-title': "Wybierz kolor"
        'data-container': "body"
        'data-toggle': "popover"
        'data-placement': "bottom"
        'data-html': "true"
        'data-content': "#{popoverContent}"
      )
      .style(
          width: "30px"
          height: "30px"
      )

  _createPopoverContent: =>
    colors = ['#64B3E0', '#9DE35A', '#FEE161', '#FEC172', '#FD605E', '#9E45B9',
      '#499CCA', '#6FC238', '#F2D130', '#FEAA3A', '#FE2D21', '#6C2185',
      '#357DA3', '#7BAF3E', '#E3B902', '#EEA02E', '#CF232C', '#55146C',
      '#175879', '#578826', '#C79403', '#D27F15', '#AE1A15']
    colorsPerLine = 6
    currentlyInLine = 0
    content = ""
    colButton = jQuery("<button></button>")
      .addClass "btn colorpick"
      .css("background-color": col)
      .css("width": "30px")
      .css("height": "30px")
      .css("margin": "2px")
    for col in colors
      currentButton = colButton.css('background-color', col).clone(true, true)
      content += jQuery(currentButton).prop('outerHTML')
      currentlyInLine += 1
      if currentlyInLine == colorsPerLine
        content += '<br>'
        currentlyInLine = 0
    content

  _dialogShown: =>
    jQuery('#colorsPopover').popover()
    jQuery('#colorsPopover').on('shown.bs.popover', (->
      $('.colorpick').click(->
        rgbvals = $(this).css("background-color")
        hexval = _rgb2hex(rgbvals).toUpperCase()
        jQuery('#colorsPopover').css("background-color", hexval)
        jQuery('#colorsPopover').popover('hide')
      )
    ))

  _rgb2hex = (rgb) ->
    hex = (x) ->
      ('0' + parseInt(x).toString(16)).slice -2
    if /^#[0-9A-F]{6}$/i.test(rgb)
      return rgb
    rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/)
    '#' + hex(rgb[1]) + hex(rgb[2]) + hex(rgb[3])

  _dialogCancel: =>
    label: "Anuluj"
    action: (dialog) =>
      jQuery('#colorsPopover').popover('hide')
      @onCloseHandler()
      dialog.close()

  _dialogSave: =>
    label: "Zapisz"
    action: (dialog) =>
      editedName = jQuery("#dialogName").val();
      jQuery('#colorsPopover').popover('hide')

      nameOk = false
      colorOk = false
      if @_verifyName(editedName)
        nameOk = true
        jQuery("#dialogError").html("")
      else
        jQuery("#dialogError").html("Nazwa może składać się tylko z liter alfabetu angielskiego i spacji.")

      if _rgb2hex(jQuery("#colorsPopover").css('background-color')) != '#ffffff'
        jQuery("#colorError").html("")
        colorOk = true
      else
        jQuery('#colorError').html("Należy ustawić kolor eksponatu")

      if nameOk && colorOk
        @_sendDataToHandler()
        dialog.close()

  _verifyName: (editedName) =>
      namePattern = /^[a-zA-Z\ ]+$/
      if editedName.match(namePattern)
        return true
      else
        return false

  _sendDataToHandler: =>
    editedName = jQuery("#dialogName").val();
    if jQuery("#dialogOption1").hasClass("active")
      floorVal = 0
    else if jQuery("#dialogOption2").hasClass("active")
      floorVal = 1
    exhibitColorHex = _rgb2hex(jQuery("#colorsPopover").css("background-color")).substring(1)
    changedData =
      name: editedName
      floor: floorVal ? null
    @_onSaveHandler(changedData)
