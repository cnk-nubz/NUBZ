root = exports ? this
root.ChangeMapDialog = class ChangeMapDialog extends root.View
  # ========== ATTACHED EVENTS ==========
  # changeFloorMap :: (Event, FormData) -> _
  # removeFloor    :: () -> _
  # =====================================
  ###
  # _dialogInfo :: {
  #   addFloor              :: String,
  #   removeFloor           :: String,
  #   processingMap         :: String,
  #   chooseMapChangeAction :: String,
  #   changeImage           :: String
  # }
  ###
  constructor: (@_dialogInfo) ->
    super()
    BootstrapDialog.show(
      message: @_dialogInfo.chooseMapChangeAction
      title: 'Wybór akcji'
      closable: false
      onshown: @_dialogShown
      buttons: [
        @_cancelButton()
      ]
    )
    @mapData = new root.MapDataHandler()


  # _dialogShown: BootstrapDialog -> undefined
  _dialogShown: (dialog) =>
    jQuery('#changeMapDialog td:first-child button').click( =>
      dialog.close()
      BootstrapDialog.show(
        message: @_dialogInfo.addFloor
        closable: false
        title: 'Mapa nowego piętra'
        buttons: [
          @_cancelButton()
          @_addFloorSaveButton()
        ]
      )
    )
    jQuery('#changeMapDialog td:first-child + td button').click( =>
      dialog.close()
      BootstrapDialog.show(
        message: @_dialogInfo.changeImage
        closable: false
        title: 'Mapa aktualnego piętra'
        buttons: [
          @_cancelButton()
          @_changeMapSaveButton()
        ]
      )
    )
    jQuery('#changeMapDialog td:last-child button').click( =>
      dialog.close()
      BootstrapDialog.show(
        message: @_dialogInfo.removeFloor
        closable: false
        title: 'Usuwanie aktualnego piętra'
        buttons: [
          @_cancelButton()
          @_removeFloorOKButton()
        ]
      )
    )
    return

  # _cancelButton :: () -> BootstrapDialogButton
  _cancelButton: ->
    label: 'Anuluj'
    action: (dialog) ->
      dialog.close()


  # _changeMapSaveButton :: () -> BootstrapDialogButton
  _changeMapSaveButton: =>
    label: 'Wyślij'
    action: (dialog) =>
      @_processMap(dialog)
      return

  # _removeFloorOKButton :: () -> BootstrapDialogButton
  _removeFloorOKButton: =>
    label: 'OK'
    action: (dialog) =>
      jQuery.ajaxSetup(
        headers: { "X-CSRFToken": getCookie("csrftoken") }
      )
      jQuery.post('removeFloor/', floor: @mapData.activeFloor, (data) =>
        if data.success
          @fireEvents("removeFloor")
          return
        BootstrapDialog.show(
          message: data.message
          type: BootstrapDialog.TYPE_DANGER
          title: "Wystąpił błąd"
        )
      )
      return


  # _addFloorSaveButton :: () -> BootstrapDialogButton
  _addFloorSaveButton: =>
    label: 'Wyślij'
    action: (dialog) =>
      newFloor = 1 + @mapData.numberOfFloors
      jQuery('#dialogForm input[name=floor]').attr('value', newFloor)
      @_processMap(dialog)
      return


  # _processMap :: BootstrapDialog -> undefined
  _processMap: (dialog) =>
    if jQuery("#dialogImage").val() is ""
      return
    dialog.enableButtons false
    jQuery("#dialogForm").off("submit")
    formData = new FormData(jQuery("#dialogForm")[0])
    jQuery("#dialogForm").submit((e) => @fireEvents("changeFloorMap", e, formData))
    jQuery("#dialogSubmit").click()
    dialog.setMessage(@_dialogInfo.processingMap)
    dialog.options.buttons = []
    dialog.updateButtons()
    return
