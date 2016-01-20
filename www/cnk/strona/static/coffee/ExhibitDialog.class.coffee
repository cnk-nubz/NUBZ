root = exports ? this
root.ExhibitDialog = class ExhibitDialog
  nameEditable: false
  onCloseHandler: -> return
  constructor: (@_name, @_floor, @_onSaveHandler) ->

  show: =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      dataType: "json"
      url: '/getExhibitDialog/'
      success: (data) =>
        @_data = data.data
        @_dialog = new BootstrapDialog(
          title: if @_name then "Edycja eksponatu" else "Tworzenie eksponatu"
          message: data.html
          closable: false
          onshown: @_addDialogHandlers
          buttons: [@_dialogCancel(), @_dialogSave()]
        )
        @_dialog.realize()
        @_dialog.open()
    )

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
      editedName.match(/^[a-zA-Z\ ]+$/)

  _addDialogHandlers: =>
    dialog = d3.select "#dialog"
    dialog.select ".inputError"
      .attr(id: "dialogError")
      .style(color: "#D8000C")

    dialog.select "input"
      .attr(
        id: "dialogName"
        value: @_name
        readonly: "true" unless @nameEditable
      )
    radioGroup = @_data[1][1].radioGroup
    floor = @_floor
    d3.selectAll "label.#{radioGroup}"
      .each( (_, i) ->
        el = d3.select(this)
        el.attr(id: "#{radioGroup}#{i}")
        el.classed("active", true) if (+el.text() is floor) or (floor is null and isNaN(+el.text()))
      )
    return

  _sendDataToHandler: =>
    editedName = jQuery("#dialogName").val()
    radioGroup = @_data[1][1].radioGroup
    if jQuery("##{radioGroup}0").hasClass("active")
      floorVal = 0
    else if jQuery("##{radioGroup}1").hasClass("active")
      floorVal = 1
    changedData =
      name: editedName
      floor: floorVal ? null
    @_onSaveHandler(changedData)
