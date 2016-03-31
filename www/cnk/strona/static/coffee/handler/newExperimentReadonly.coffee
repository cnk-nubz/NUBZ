root = exports ? this
class Handlers
  constructor: (@_experimentData) ->
    @_DOM =
      experimentTitle: ".experimentTitle input"
    @_questionsBefore = new root.Questions(@_experimentData?.questionsBefore)
    @_questionsAfter = new root.Questions(@_experimentData?.questionsAfter)
    @_exhibitActions = new root.Actions(@_experimentData?.exhibitActions)
    @_breakActions = new root.Actions(@_experimentData?.breakActions)
    @_experimentQuestionRow = new root.ReadonlyExperimentQuestionRow()
    @_experimentActionRow = new root.ReadonlyExperimentActionRow()
    @_setLists()
    @_setDefaultState()

  _setLists: =>
    questionsBeforeDOM = @_questionsBefore.getAllElementsAsDOM(@_experimentQuestionRow)
    questionsAfterDOM = @_questionsAfter.getAllElementsAsDOM(@_experimentQuestionRow)
    exhibitActionsDOM = @_exhibitActions.getAllElementsAsDOM(@_experimentActionRow)
    breakActionsDOM = @_breakActions.getAllElementsAsDOM(@_experimentActionRow)
    questionsBeforeDOM = @_prepareExperimentList(questionsBeforeDOM, @_questionsBefore)
    questionsAfterDOM = @_prepareExperimentList(questionsAfterDOM, @_questionsAfter)
    exhibitActionsDOM = @_prepareExperimentList(exhibitActionsDOM, @_exhibitActions)
    breakActionsDOM = @_prepareExperimentList(breakActionsDOM, @_breakActions)
    @_questionsBeforeList = new root.QuestionsList('.questionsBefore .middle', null, questionsBeforeDOM)
    @_questionsAfterList = new root.QuestionsList('.questionsAfter .middle', null, questionsAfterDOM)
    @_exhibitActionsList = new root.ActionsList('.experimentActions .middle', null, exhibitActionsDOM)
    @_breakActionsList = new root.ActionsList('.breakActions .middle', null, breakActionsDOM)

  _setDefaultState: =>
    jQuery(@_DOM.experimentTitle).val(@_experimentData.name)
    @_questionsBeforeList.show()
    @_questionsAfterList.show()
    @_exhibitActionsList.show()
    @_breakActionsList.show()

  _prepareExperimentList: (elementsDOM, dataList) =>
    [].forEach.call(elementsDOM.querySelectorAll("tr"), (element) =>
      viewId = element.data
      element.querySelector("td:first-child")
        .addEventListener("click", ->
          dataList.showDialog(viewId, true)
        )
    )
    elementsDOM


jQuery(document).ready( ->
  new Handlers(root.experimentData)
)
