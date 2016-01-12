root = exports ? this
root.ExhibitDialog = class ExhibitDialog
  nameEditable: false
  constructor: (@_name, @_floor, @_handler) ->

  show: =>
    _message =
      d3.select(document.createElement("div"))
      .append "div"
        .classed "container-fluid", true
    @_createFirstRow(_message, @_name, @nameEditable)
    @_createSecondRow(_message)
    _message = _message[0][0].outerHTML
    _dialog_buttons = [@_dialogCancel(), @_dialogSave()]
    @_dialog = new BootstrapDialog(
      title: "Edycja eksponatu"
      message: _message
      closable: false
      buttons: _dialog_buttons
    )
    @_dialog.realize()
    @_dialog.open()

  _createFirstRow: (parent, exhibitName, isNameEditable) =>
    line1 = parent
      .append "form"
        .classed "form-group row", true
    line1Title = line1
      .append "div"
        .classed "col-sm-2", true
    line1Title
      .append "label"
        .text("Nazwa:")
        .attr(
          for: "dialogName"
        )
        .style(
          "padding-top" : "0.5em"
        )
    line1Input = line1
      .append "div"
        .classed "col-sm-10", true
    line1Input
      .append "input"
        .attr(
          id: "dialogName"
          type: "text"
          value: exhibitName
          disabled: "true" unless isNameEditable
        )
        .classed "form-control", true
    line1Input
      .append "div"
        .attr(
          id: "dialogError"
        )

  _createSecondRow: (parent) =>
    line2 = parent
      .append "div"
        .style(
          "margin-top" : "0.5em"
        )
        .classed "row", true
    line2Label = line2
      .append "div"
        .classed "col-sm-2", true
    line2Label
      .append "label"
        .text("Piętro:")
        .style(
          "padding-top" : "0.5em"
        )
    line2Radios = line2
      .append "div"
        .attr(
          role: "group"
          'data-toggle': "buttons"
        )
        .classed "btn-group col-sm-10", true
    @_createRadio(line2Radios, "radio1", @_floor is 0, "0")
    @_createRadio(line2Radios, "radio2", @_floor is 1, "1")
    @_createRadio(line2Radios, "radio3", @_floor is null, "brak")

  _createRadio: (parent, radioId, isActive, radioText) =>
    radioLabel = parent
      .append "label"
        .attr(
          id: radioId
        )
        .text radioText
        .classed "btn btn-default #{if isActive then "active" else ""}", true
    radioLabel
      .append "input"
        .attr(
          type: "radio"
          name: "rfloor"
        )

  _dialogCancel: =>
    label: "Anuluj"
    action: (dialog) =>
      @_sendDataToHandler(true)
      dialog.close()

  _dialogSave: =>
    label: "Zapisz"
    action: (dialog) =>
      editedName = jQuery("#dialogName").val();
      if @_verifyName(editedName) is true
        @_sendDataToHandler(false)
        dialog.close()
      else
        jQuery("#dialogError").html("<span style=\"color: #D8000C\">Nazwa może składać się tylko z liter alfabetu angielskiego i spacji.</span>")

  _verifyName: (editedName) =>
      namePattern = /^[a-zA-Z\ ]+$/
      if editedName.match(namePattern)
        return true
      else
        return false

  _sendDataToHandler: (wasCanceled) =>
    editedName = jQuery("#dialogName").val();
    if jQuery("#radio1").hasClass("active")
      floorVal = 0
    else if jQuery("#radio2").hasClass("active")
      floorVal = 1
    changedData =
      name: editedName
      floor: floorVal ? null
    @_handler(changedData, wasCanceled)
