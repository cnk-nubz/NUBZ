root = exports ? this
class Handler
  constructor: (_readyExperiments, _finishedExperiments, @_activeExperiment) ->
    @_readyExperiments = new root.ReadyExperiments(_readyExperiments)
    @_finishedExperiments = new root.FinishedExperiments(_finishedExperiments)
    @_readyExperimentRow = new root.ReadyExperimentRow()
    @_finishedExperimentRow = new root.FinishedExperimentRow()
    @_setHandlers()
    @_setDefaultState()

  _setHandlers: =>
    @_setLists()
    activeExperimentRow = document.querySelector('#activeExperiment tr')
    activeExperimentRow.querySelector("td:not(:first-child):not(:last-child)").addEventListener("click", ->
      experimentId = this.parentNode.data
      alert("TODO REDIRECT TO READONLY EXPERIMENT WITH ID: #{experimentId}")
    )
    activeExperimentRow.querySelector("td:last-child").addEventListener("click", ->
      experimentId = this.parentNode.data
      alert("TODO FINISH EXPERIMENT WITH ID: #{experimentId}")
    )

  _setLists: =>
    readyExperimentsDOM = @_prepareReadyExperiments()
    finishedExperimentsDOM = @_prepareFinishedExperiments()
    @_readyExperimentsList = new root.ExperimentsList('#readyExperiments .middle', 'questionsActionsTable', readyExperimentsDOM)
    @_finishedExperimentsList = new root.ExperimentsList('#finishedExperiments .middle', 'questionsActionsTable', finishedExperimentsDOM)
    return

  _prepareReadyExperiments: =>
    experimentsDOM = @_readyExperiments.getAllElementsAsDOM(@_readyExperimentRow)
    [].forEach.call(experimentsDOM.querySelectorAll("tr"), (element) ->
      viewId = element.data
      element.querySelector("td:not(:first-child):not(:last-child)")
        .addEventListener("click", =>
          alert("TODO REDIRECT TO EXPERIMENT WITH VIEWID #{viewId}")
        )
      element.querySelector("td:last-child")
        .addEventListener("click", =>
          alert("TODO ACTIVATE EXPERIMENT WITH VIEWID #{viewId}")
        )
    )
    experimentsDOM

  _prepareFinishedExperiments: =>
    experimentsDOM = @_finishedExperiments.getAllElementsAsDOM(@_finishedExperimentRow)
    # TODO: add cloning of experiment

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
