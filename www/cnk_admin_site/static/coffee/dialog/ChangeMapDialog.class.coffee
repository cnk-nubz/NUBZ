root = exports ? this
root.ChangeMapDialog = class ChangeMapDialog extends root.View
  # ========== ATTACHED EVENTS ==========
  # addFloor       :: (Event, FormData) -> _
  # changeFloorMap :: (Event, FormData) -> _
  # removeFloor    :: () -> _
  # =====================================
  # _dialogInfo :: {
  #   addFloor              :: String,
  #   removeFloor           :: String,
  #   processingMap         :: String,
  #   chooseMapChangeAction :: String,
  #   changeImage           :: String
  # }
  #
  # _canvasInfo :: {
  #   activeFloor    :: Int,
  #   numberOfFloors :: Int
  # }
  constructor: (@_dialogInfo, @_canvasInfo) ->
    super()
    BootstrapDialog.show(
      message: @_dialogInfo.chooseMapChangeAction
      title: 'Wybór akcji'
      closable: false
      onshown: @_dialogShown
      size: BootstrapDialog.SIZE_SMALL
      buttons: [
        @_cancelButton()
      ]
    )


  # _dialogShown: BootstrapDialog -> undefined
  _dialogShown: (dialog) =>
    jQuery('#changeMapDialog td:first-child button').click( =>
      dialog.close()
      BootstrapDialog.show(
        message: @_dialogInfo.addFloor
        closable: false
        title: 'Mapa nowego piętra'
        size: BootstrapDialog.SIZE_SMALL
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
        size: BootstrapDialog.SIZE_SMALL
        title: 'Mapa aktualnego piętra'
        buttons: [
          @_cancelButton()
          @_changeMapSaveButton()
          @_removeFloorButton() if @_canvasInfo.numberOfFloors is 1 + @_canvasInfo.activeFloor
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
      jQuery("#dialogForm input[name=floor]").attr('value', @_canvasInfo.activeFloor)
      @_processMap(dialog)
      return


  # _removeFloorButton: :: () -> BootstrapDialogButton
  _removeFloorButton: =>
    label: 'Usuń'
    cssClass: 'delete-button btn-danger'
    action: (dialog) =>
      BootstrapDialog.show(
        message: @_dialogInfo.removeFloor
        title: 'Potwierdzenie usunięcia piętra'
        closable: false
        buttons: [
          @_cancelButton()
          @_removeFloorConfirmationButton()
        ]
      )

  # _removeFloorConfirmationButton :: () -> BootstrapDialogButton
  _removeFloorConfirmationButton: =>
    label: 'OK'
    action: (dialog) =>
      jQuery.ajaxSetup(
        headers: { "X-CSRFToken": getCookie("csrftoken") }
      )
      jQuery.ajax(
        method: "POST"
        data: (floor: @_canvasInfo.activeFloor)
        url: '/removeFloor/'
        success: => @fireEvents("removeFloor")
        error: @_displayError
      )
      return


  # _addFloorSaveButton :: () -> BootstrapDialogButton
  _addFloorSaveButton: =>
    label: 'Wyślij'
    action: (dialog) =>
      newFloor = @_canvasInfo.numberOfFloors
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


  # _displayError :: jqXHR -> undefined
  _displayError: (obj) ->
    BootstrapDialog.show(
      message: obj.responseText
      title: obj.statusText
      type: BootstrapDialog.TYPE_DANGER
    )
    return
