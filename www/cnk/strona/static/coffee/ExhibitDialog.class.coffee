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
    @_appendOption(floorOptions, "dialogOption3", "brak", @_floor is null)
    ## configuration
    floorOptions.attr(
      role: "group"
      'data-toggle': "buttons"
    )

  _appendHorizontalForm: (parent) ->
    parent.append "form"
      .classed "form-horizontal", true
      .attr(
        role: "form"
      )

  _appendFormLine: (form) ->
    form.append "div"
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

  _dialogCancel: =>
    label: "Anuluj"
    action: (dialog) =>
      @onCloseHandler()
      dialog.close()

  _dialogSave: =>
    label: "Zapisz"
    action: (dialog) =>
      editedName = jQuery("#dialogName").val();
      if @_verifyName(editedName)
        @_sendDataToHandler()
        dialog.close()
      else
        jQuery("#dialogError").html("Nazwa może składać się tylko z liter alfabetu angielskiego i spacji.")

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
    changedData =
      name: editedName
      floor: floorVal ? null
    @_onSaveHandler(changedData)