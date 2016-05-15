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
          experimentId = JSON.parse(viewId).id
          (new CloneExperimentDialog())
            .on('save', @_tryCloningExperiment(experimentId))
            .show()
        )

      if @_activeExperiment?
        jQuery("td:last-child", element).addClass("nonclickable noselect")
        jQuery("td:last-child > div", element).tooltip()
      else
        element.querySelector("td:last-child > div")
          .addEventListener("click", =>
            (new ConfirmationDialog(root.startExperimentConfirmation))
              .on('confirm', @_startExperiment(JSON.parse(viewId).id))
          )
    )
    experimentsDOM


  # _startExperiment :: Int -> () -> undefined
  _startExperiment: (experimentId) =>
    =>
      jQuery.ajaxSetup(
        headers: { "X-CSRFToken": getCookie("csrftoken") }
      )
      jQuery.ajax(
        method: "POST"
        data: (jsonData: JSON.stringify(experimentId: experimentId))
        url: "/activeExperiment/"
        success: -> location.reload()
        error: @_displayError
      )
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
          experimentId = JSON.parse(viewId).id
          (new CloneExperimentDialog())
            .on('save', @_tryCloningExperiment(experimentId))
            .show()
        )
    )
    experimentsDOM


  # _tryCloningExperiment :: Int -> (CloneExperimentData, BootstrapDialog) -> undefined
  _tryCloningExperiment: (experimentId) =>
    (data, dialog) =>
      jQuery('.inputError', dialog.getModalBody()).html('')
      toSend =
        jsonData: JSON.stringify(
          experimentId: experimentId
          newName: data.name
        )
      jQuery.ajaxSetup(
        headers: { "X-CSRFToken": getCookie("csrftoken") }
      )
      jQuery.ajax(
        method: "POST"
        data: toSend
        dataType: 'json'
        url: '/cloneExperiment/'
        statusCode:
          403: -> dialog.showNameDuplicatedError()
          500: @_displayError
        success: -> location.reload()
      )
      return


  # _displayError :: jqXHR -> undefined
  _displayError: (obj) ->
    BootstrapDialog.show(
      message: obj.responseText
      title: obj.statusText
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
    activeExperimentRow.querySelector("td:not(:first-child):not(:last-child)")
      .addEventListener("click", ->
        location.href = "/badanie?readonly&id=#{activeExperimentId}"
      )
    activeExperimentRow.querySelector("td:last-child > div")
      .addEventListener("click", =>
        (new root.ConfirmationDialog(root.finishExperimentConfirmation))
          .on('confirm', @_finishExperiment)
      )
    return


  # _confirmationCancelButton :: () -> BootstrapDialogButton
  _confirmationCancelButton: ->
    label: root.confirmationMessages.cancelButton
    action: (dialog) ->
      dialog.close()


  # _confirmationConfirmButton :: (() -> undefined) -> BootstrapDialogButton
  _confirmationConfirmButton: (handler) ->
    label: root.confirmationMessages.confirmationButton
    action: -> handler()


  # _finishExperiment :: () -> undefined
  _finishExperiment: =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      method: "DELETE"
      url: "/activeExperiment/"
      success: -> location.reload()
      error: @_displayError
    )
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
