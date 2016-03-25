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
      element.querySelector("td:not(:first-child):not(:last-child)")
        .addEventListener("click", =>
          location.href = "/badanie?id=#{JSON.parse(viewId).id}"
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
    # TODO: add cloning of experiment

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
      activeExperimentRow.data = @_activeExperiment.experimentId
      activeExperimentRow.querySelector("td:first-child > div > span").innerHTML = @_activeExperiment.name
    else
      activeExperimentRow.querySelector("td:first-child > div > span").innerHTML = "Brak aktywnego badania"
    @_readyExperimentsList.show()
    @_finishedExperimentsList.show()

jQuery(document).ready( ->
  new Handler(root.readyExperiments, root.finishedExperiments, root.activeExperiment)
)
