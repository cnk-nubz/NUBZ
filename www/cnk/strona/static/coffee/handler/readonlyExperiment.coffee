root = exports ? this
class Handlers
  constructor: (@_experimentData, @_raportsData) ->
    @_DOM =
      experimentTitle: ".experimentTitle input"
      getAllRaports: ".raportsList .myFooter button"
    @_questionsBefore = new root.Questions(@_experimentData?.questionsBefore)
    @_questionsAfter = new root.Questions(@_experimentData?.questionsAfter)
    @_exhibitActions = new root.Actions(@_experimentData?.exhibitActions)
    @_breakActions = new root.Actions(@_experimentData?.breakActions)
    @_raports = new root.Raports(@_raportsData)
    @_experimentQuestionRow = new root.ReadonlyExperimentQuestionRow()
    @_experimentActionRow = new root.ReadonlyExperimentActionRow()
    @_raportsRow = new root.RaportRow()
    @_setLists()
    @_setDefaultState()
    @_setSaveAllRaportsHandler()

  _setLists: =>
    questionsBeforeDOM = @_prepareExperimentList(@_experimentQuestionRow, @_questionsBefore)
    questionsAfterDOM = @_prepareExperimentList(@_experimentQuestionRow, @_questionsAfter)
    exhibitActionsDOM = @_prepareExperimentList(@_experimentActionRow, @_exhibitActions)
    breakActionsDOM = @_prepareExperimentList(@_experimentActionRow, @_breakActions)
    raportsDOM = @_prepareRaportsList(@_raportsRow, @_raports)
    @_questionsBeforeList = new root.QuestionsList('.questionsBefore .middle', null, questionsBeforeDOM)
    @_questionsAfterList = new root.QuestionsList('.questionsAfter .middle', null, questionsAfterDOM)
    @_exhibitActionsList = new root.ActionsList('.experimentActions .middle', null, exhibitActionsDOM)
    @_breakActionsList = new root.ActionsList('.breakActions .middle', null, breakActionsDOM)
    @_raportsList = new root.RaportsList('.raportsList .middle', 'raportsTable', raportsDOM)

  _setDefaultState: =>
    jQuery(@_DOM.experimentTitle).val(@_experimentData.name)
    @_questionsBeforeList.show()
    @_questionsAfterList.show()
    @_exhibitActionsList.show()
    @_breakActionsList.show()
    @_raportsList.show()

  _prepareExperimentList: (rowFactory, dataList) =>
    elementsDOM = dataList.getAllElementsAsDOM(rowFactory)
    [].forEach.call(elementsDOM.querySelectorAll("tr"), (element) =>
      viewId = element.data
      element.querySelector("td:first-child")
        .addEventListener("click", ->
          dataList.showDialog(viewId, true)
        )
    )
    elementsDOM

  _prepareRaportsList: (rowFactory, dataList) =>
    elementsDOM = dataList.getAllElementsAsDOM(rowFactory)
    [].forEach.call(elementsDOM.querySelectorAll("tr"), (element) =>
      viewId = element.data
      element.querySelector("td:first-child")
        .addEventListener('click', (e) ->
          location.replace("getRaport?id=#{JSON.parse(viewId).id}")
        )
    )
    elementsDOM

  _setSaveAllRaportsHandler: =>
    jQuery(@_DOM.getAllRaports).click( ->
      location.replace('getAllRaports/')
    )

jQuery(document).ready( ->
  new Handlers(root.experimentData, root.raportsList)
)
