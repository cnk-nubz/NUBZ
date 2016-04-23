root = exports ? this
root.cachedData = {}
class Handler
  ###
  # type ActiveExperiment = {
  #   startDate    :: Date,
  #   name         :: String,
  #   experimentId :: Int
  # }
  ###
  # constructor :: ([ReadyExperiment], [FinishedExperiment], ActiveExperiment)
  constructor: (_readyExperiments, _finishedExperiments, @_activeExperiment) ->
    @_DOM =
      addNewExperiment: '#readyExperiments .addElement'
    @_readyExperiments = new root.ReadyExperiments(_readyExperiments)
    @_finishedExperiments = new root.FinishedExperiments(_finishedExperiments)
    @_readyExperimentRow = new root.ReadyExperimentRow()
    @_finishedExperimentRow = new root.FinishedExperimentRow()
    @_setHandlers()
    @_setDefaultState()


  # _setHandler :: () -> undefined
  _setHandlers: =>
    @_setLists()
    @_setAddNewExperimentHandler()
    @_setActiveExperimentHandlers()
    return


  # _setLists :: () -> undefined
  _setLists: =>
    readyExperimentsDOM = @_prepareReadyExperiments()
    finishedExperimentsDOM = @_prepareFinishedExperiments()
    @_readyExperimentsList = new root.ExperimentsList('#readyExperiments .middle', 'readyExperimentsTable', readyExperimentsDOM)
    @_finishedExperimentsList = new root.ExperimentsList('#finishedExperiments .middle', 'finishedExperimentsTable', finishedExperimentsDOM)
    return


  # _prepareReadyExperiments :: () -> DocumentFragment
  _prepareReadyExperiments: =>
    experimentsDOM = @_readyExperiments.getAllElementsAsDOM(@_readyExperimentRow)
    [].forEach.call(experimentsDOM.querySelectorAll("tr"), (element) =>
      viewId = element.data
      element.querySelector("td:first-child + td > div")
        .addEventListener("click", ->
          location.href = "/badanie?id=#{JSON.parse(viewId).id}"
        )
      element.querySelector("td:first-child + td + td > div")
        .addEventListener("click", =>
          @_proceedCloningExperiment(JSON.parse(viewId).id)
        )

      if @_activeExperiment?
        jQuery("td:last-child", element).addClass("nonclickable noselect")
        jQuery("td:last-child > div", element).tooltip()
      else
        element.querySelector("td:last-child > div")
        .addEventListener("click", =>
          BootstrapDialog.show(
            message: root.confirmationMessages.activateExperiment
            title: root.confirmationMessages.title
            closable: false
            buttons: [
              @_confirmationCancelButton()
              @_confirmationConfirmButton(@_activateExperiment(JSON.parse(viewId).id))
            ]
          )
        )
    )
    experimentsDOM


  # _activateExperiment :: Int -> () -> undefined
  _activateExperiment: (experimentId) ->
    ->
      jQuery.ajaxSetup(
        headers: { "X-CSRFToken": getCookie("csrftoken") }
      )
      jQuery.post("startExperiment/", id: experimentId, (data) ->
        if data.success
          location.reload()
        else
          BootstrapDialog.show(
            message: data.message
            title: 'Nie mozna rozpoczac badania'
            type: BootstrapDialog.TYPE_DANGER
          )
      , 'json')
      return


  # _prepareFinishedExperiments :: () -> DocumentFragment
  _prepareFinishedExperiments: =>
    experimentsDOM = @_finishedExperiments.getAllElementsAsDOM(@_finishedExperimentRow)
    [].forEach.call(experimentsDOM.querySelectorAll("tr"), (element) =>
      viewId = element.data
      element.querySelector('td:first-child + td + td > div')
        .addEventListener('click', ->
          location.href = "/badanie?readonly&id=#{JSON.parse(viewId).id}"
        )
      element.querySelector('td:first-child + td + td + td > div')
        .addEventListener('click', =>
          @_proceedCloningExperiment(JSON.parse(viewId).id)
        )
    )
    experimentsDOM


  # _proceedCloningExperiment :: Int -> undefined
  _proceedCloningExperiment: (experimentId) =>
    dialogData = root.changeNameDialog.data
    dialogMessage = root.changeNameDialog.html
    BootstrapDialog.show(
      message: dialogMessage
      title: dialogData.utils.text.title
      closable: false
      buttons: [
        @_changeNameDialogCancelButton()
        @_changeNameDialogSaveButton(@_tryCloningExperiment(experimentId))
      ]
    )
    return


  # _changeNameDialogCancelButton :: () -> BootstrapDialogButton
  _changeNameDialogCancelButton: ->
    label: root.changeNameDialog.data.utils.text.cancelButton
    action: (dialog) ->
      dialog.close()


  # _changeNameDialogSaveButton :: Int -> BootstrapDialogButton
  _changeNameDialogSaveButton: (handler) ->
    label: root.changeNameDialog.data.utils.text.saveButton
    action: (dialog) -> handler()


  # _tryCloningExperiment :: Int -> () -> undefined
  _tryCloningExperiment: (experimentId) =>
    =>
      jQuery('.inputError').html('')
      newName = jQuery.trim(jQuery('#dialog input').val())
      jQuery("#dialog input").val(newName)
      dialogData = root.changeNameDialog.data
      if newName.length is 0
        jQuery('.inputError').text(dialogData.utils.text.emptyInputError)
        return
      toSend =
        jsonData: JSON.stringify(
          experimentId: experimentId
          newName: newName
        )
      jQuery.ajaxSetup(
        headers: { "X-CSRFToken": getCookie("csrftoken") }
      )
      jQuery.post('cloneExperiment/', toSend, (data) =>
        if data.success is true
          location.reload()
        else
          if data.exceptionType is 'DuplicateName'
            jQuery('.inputError').html(dialogData.utils.text.nameDuplicatedError)
          else
            @_showError(data.exceptionType, data.message)
      )
      return


  # _showError :: (String, String) -> undefined
  _showError: (exceptionType, message) ->
    BootstrapDialog.show(
      message: message
      title: exceptionType
      type: BootstrapDialog.TYPE_DANGER
    )
    return


  # _setAddNewExperimentHandler :: () -> undefined
  _setAddNewExperimentHandler: =>
    jQuery(@_DOM.addNewExperiment).click( ->
      location.href = "/badanie"
    )
    return


  # _setActiveExperimentHandlers :: () -> undefined
  _setActiveExperimentHandlers: =>
    if not @_activeExperiment?
      return
    jQuery("td:not(:first-child) > div").addClass("noselect")
    activeExperimentRow = document.querySelector('#activeExperiment tr')
    activeExperimentId = @_activeExperiment.experimentId
    activeExperimentRow.querySelector("td:not(:first-child):not(:last-child)").addEventListener("click", ->
      location.href = "/badanie?readonly&id=#{activeExperimentId}"
    )
    activeExperimentRow.querySelector("td:last-child > div").addEventListener("click", =>
      BootstrapDialog.show(
        message: root.confirmationMessages.finishExperiment
        title: root.confirmationMessages.title
        closable: false
        buttons: [
          @_confirmationCancelButton()
          @_confirmationConfirmButton(@_finishExperiment)
        ]
      )

    )
    return


  # _confirmationCancelButton :: () -> BootstrapDialogButton
  _confirmationCancelButton: ->
    label: root.confirmationMessages.cancelButton
    action: (dialog) ->
      dialog.close()


  # _confirmationConfirmButton :: (() -> undefined) -> BootstrapDialogButton
  _confirmationConfirmButton: (handler) ->
    label: root.confirmationMessages.confirmButton
    action: -> handler()


  # _finishExperiment :: () -> undefined
  _finishExperiment: ->
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.post("finishExperiment/", null, (data) ->
      if data.success
        location.reload()
      else
        BootstrapDialog.show(
          message: data.message
          title: 'Nie mozna zakonczyc badania'
          type: BootstrapDialog.TYPE_DANGER
        )
    , 'json')
    return


  # _setDefaultState :: () -> undefined
  _setDefaultState: =>
    activeExperimentRow = document.querySelector('#activeExperiment tr')
    if @_activeExperiment?
      activeExperimentRow.data = @_activeExperiment.experimentId
      oldHeaderText = jQuery('#activeExperiment > div').text()
      newHeaderText = "#{oldHeaderText} (#{@_activeExperiment.startDate} - )"
      jQuery('#activeExperiment > div').text(newHeaderText)
      jQuery("td:first-child > div > span").html("#{@_activeExperiment.name}").shortenText()
    else
      activeExperimentRow.querySelector("td:first-child > div > span").innerHTML = "Brak aktywnego badania"
    @_readyExperimentsList.show()
    @_finishedExperimentsList.show()
    return

jQuery(document).ready( ->
  new Handler(root.readyExperiments, root.finishedExperiments, root.activeExperiment)
)
