root = exports ? this
class Handler
  constructor: (_readyExperiments, _finishedExperiments, @_activeExperiment) ->
    @_DOM =
      addNewExperiment: '#readyExperiments .addElement'
    @_readyExperiments = new root.ReadyExperiments(_readyExperiments)
    @_finishedExperiments = new root.FinishedExperiments(_finishedExperiments)
    @_readyExperimentRow = new root.ReadyExperimentRow()
    @_finishedExperimentRow = new root.FinishedExperimentRow()
    @_setHandlers()
    @_setDefaultState()

  _setHandlers: =>
    @_setLists()
    @_setAddNewExperimentHandler()
    @_setActiveExperimentHandlers()

  _setLists: =>
    readyExperimentsDOM = @_prepareReadyExperiments()
    finishedExperimentsDOM = @_prepareFinishedExperiments()
    @_readyExperimentsList = new root.ExperimentsList('#readyExperiments .middle', 'readyExperimentsTable', readyExperimentsDOM)
    @_finishedExperimentsList = new root.ExperimentsList('#finishedExperiments .middle', 'finishedExperimentsTable', finishedExperimentsDOM)
    return

  _prepareReadyExperiments: =>
    experimentsDOM = @_readyExperiments.getAllElementsAsDOM(@_readyExperimentRow)
    [].forEach.call(experimentsDOM.querySelectorAll("tr"), (element) =>
      viewId = element.data
      element.querySelector("td:first-child + td > div")
        .addEventListener("click", =>
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
          jQuery.ajaxSetup(
            headers: { "X-CSRFToken": getCookie("csrftoken") }
          )
          jQuery.post("startExperiment/", id: JSON.parse(viewId).id, (data) =>
            if data.success
              location.reload()
            else
              BootstrapDialog.show(
                message: data.message
                title: 'Nie mozna rozpoczac badania'
                type: BootstrapDialog.TYPE_DANGER
              )
          , 'json')
        )
    )
    experimentsDOM

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

  _proceedCloningExperiment: (experimentId) =>
    dialogData = root.changeNameDialog.data
    dialogMessage = root.changeNameDialog.html
    BootstrapDialog.show(
      message: dialogMessage
      title: dialogData.utils.text.title
      closable: false
      buttons: [
        @_changeNameDialogCancelButton()
        @_changeNameDialogSaveButton(experimentId)
      ]
    )

  _changeNameDialogCancelButton: ->
    dialogData = root.changeNameDialog.data
    label: dialogData.utils.text.cancelButton
    action: (dialog) ->
      dialog.close()

  _changeNameDialogSaveButton: (experimentId) =>
    dialogData = root.changeNameDialog.data
    label: dialogData.utils.text.saveButton
    action: (dialog) =>
      jQuery('.inputError').html('')
      newName = jQuery('#dialog input').val()
      @_tryCloningExperiment(experimentId, newName)

  _tryCloningExperiment: (experimentId, newName) =>
    if newName is ''
      jQuery('.inputError').html(dialogData.utils.text.emptyInputError)
      return
    dialogData = root.changeNameDialog.data
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

  _showError: (exceptionType, message) ->
    BootstrapDialog.show(
      message: message
      title: exceptionType
      type: BootstrapDialog.TYPE_DANGER
    )
    return

  _setAddNewExperimentHandler: =>
    jQuery(@_DOM.addNewExperiment).click( ->
      location.href = "/badanie"
    )

  _setActiveExperimentHandlers: =>
    if not @_activeExperiment?
      return
    jQuery("td:not(:first-child) > div").addClass("noselect")
    activeExperimentRow = document.querySelector('#activeExperiment tr')
    activeExperimentId = @_activeExperiment.experimentId
    activeExperimentRow.querySelector("td:not(:first-child):not(:last-child)").addEventListener("click", ->
      location.href = "/badanie?readonly&id=#{activeExperimentId}"
    )
    activeExperimentRow.querySelector("td:last-child > div").addEventListener("click", ->
      jQuery.ajaxSetup(
        headers: { "X-CSRFToken": getCookie("csrftoken") }
      )
      jQuery.post("finishExperiment/", null, (data) =>
        if data.success
          location.reload()
        else
          BootstrapDialog.show(
            message: data.message
            title: 'Nie mozna zakonczyc badania'
            type: BootstrapDialog.TYPE_DANGER
          )
      , 'json')
    )
    return

  _setDefaultState: =>
    activeExperimentRow = document.querySelector('#activeExperiment tr')
    if @_activeExperiment?
      date = @_activeExperiment.startDate
      fixDate = (x) -> if 0 < x < 10 then "0#{x}" else "#{x}"
      activeExperimentDate = "(#{fixDate date.day}/#{fixDate date.month}/#{fixDate date.year} - )"
      activeExperimentRow.data = @_activeExperiment.experimentId
      oldHeaderText = jQuery('#activeExperiment > div').text()
      jQuery('#activeExperiment > div').text("#{oldHeaderText} #{activeExperimentDate}")
      jQuery("td:first-child > div > span").html("#{@_activeExperiment.name}").shortenText()
    else
      activeExperimentRow.querySelector("td:first-child > div > span").innerHTML = "Brak aktywnego badania"
    @_readyExperimentsList.show()
    @_finishedExperimentsList.show()

jQuery(document).ready( ->
  new Handler(root.readyExperiments, root.finishedExperiments, root.activeExperiment)
)
