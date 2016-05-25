root = exports ? this
class Handlers
  ###
  # type ExperimentData = {
  #   breakActions :: [Action],
  #   exhibitActions :: [Action],
  #   experimentId :: Int,
  #   name :: String,
  #   questionsAfter :: [Question],
  #   questionsBefore :: [Question]
  # }
  ###
  # constructor :: (ExperimentData, [Report]) -> Context
  constructor: (@_experimentData, @_reportsData, @_activeExperiment) ->
    @_DOM =
      experimentTitle: ".experimentTitle input"
      getAllReports: ".reportsList .myFooter button"
      removeExperiment: ".manageExperiment button"
      manageExperiment: ".manageExperiment"
    @_questionsBefore = new root.Questions(@_experimentData?.surveyBefore)
    @_questionsAfter = new root.Questions(@_experimentData?.surveyAfter)
    @_exhibitActions = new root.Actions(@_experimentData?.exhibitActions)
    @_breakActions = new root.Actions(@_experimentData?.breakActions)
    @_reports = new root.Reports(@_reportsData)
    @_experimentQuestionRow = new root.ReadonlyExperimentQuestionRow()
    @_experimentActionRow = new root.ReadonlyExperimentActionRow()
    @_reportsRow = new root.ReportRow()
    @_setLists()
    @_setDefaultState()
    @_setSaveAllReportsHandler()
    @_setRemoveExperimentHandler()


  # _setLists :: () -> undefined
  _setLists: =>
    questionsBeforeDOM = @_prepareExperimentList(@_experimentQuestionRow, @_questionsBefore)
    questionsAfterDOM = @_prepareExperimentList(@_experimentQuestionRow, @_questionsAfter)
    exhibitActionsDOM = @_prepareExperimentList(@_experimentActionRow, @_exhibitActions)
    breakActionsDOM = @_prepareExperimentList(@_experimentActionRow, @_breakActions)
    reportsDOM = @_prepareReportsList(@_reportsRow, @_reports)
    @_questionsBeforeList = new root.QuestionsList('.questionsBefore .middle', null, questionsBeforeDOM)
    @_questionsAfterList = new root.QuestionsList('.questionsAfter .middle', null, questionsAfterDOM)
    @_exhibitActionsList = new root.ActionsList('.experimentActions .middle', null, exhibitActionsDOM)
    @_breakActionsList = new root.ActionsList('.breakActions .middle', null, breakActionsDOM)
    @_reportsList = new root.ReportsList('.reportsList .middle', 'reportsTable', reportsDOM)


  # _setDefaultState :: () -> undefined
  _setDefaultState: =>
    if @_activeExperiment?.experimentId is @_experimentData.experimentId
      jQuery(@_DOM.manageExperiment).remove()
    jQuery(@_DOM.experimentTitle).val(@_experimentData.name)
    @_questionsBeforeList.show()
    @_questionsAfterList.show()
    @_exhibitActionsList.show()
    @_breakActionsList.show()
    @_reportsList.show()

  ###
  # type RowFactory =
  #   root.ExperimentQuestionRow
  # | root.ExperimentActionRow
  ###
  # _prepareExperimentList :: (RowFactory, root.ListView) -> DocumentFragment
  _prepareExperimentList: (rowFactory, dataList) ->
    elementsDOM = dataList.getAllElementsAsDOM(rowFactory)
    [].forEach.call(elementsDOM.querySelectorAll("tr"), (element) ->
      viewId = element.data
      element.querySelector("td:first-child")
        .addEventListener("click", ->
          dataList.getFilledDialog(viewId, readonly: true).show()
        )
    )
    elementsDOM


  # _prepareReportsList :: (root.ReportsRow, root.ListView) -> DocumentFragment
  _prepareReportsList: (rowFactory, dataList) ->
    elementsDOM = dataList.getAllElementsAsDOM(rowFactory)
    [].forEach.call(elementsDOM.querySelectorAll("tr"), (element) ->
      viewId = element.data
      element.querySelector("td:first-child")
        .addEventListener('click', (e) ->
          location.assign("getReport/?id=#{JSON.parse(viewId).id}")
        )
    )
    elementsDOM


  # _setSaveAllReportsHandler :: () -> undefined
  _setSaveAllReportsHandler: =>
    jQuery(@_DOM.getAllReports).click( =>
      location.assign("getAllReports/?id=#{@_experimentData.experimentId}")
    )
    return


  # _setRemoveExperimentHandler :: () -> undefined
  _setRemoveExperimentHandler: =>
    jQuery(@_DOM.removeExperiment).click( =>
      (new root.ConfirmationDialog(root.structures.dialog.removeExperimentConfirmation))
        .on('confirm', =>
          experimentId = @_experimentData.experimentId
          jQuery.ajaxSetup(
            headers: { "X-CSRFToken": getCookie("csrftoken") }
          )
          jQuery.ajax(
            method: "POST"
            data: (jsonData: JSON.stringify(experimentId: experimentId))
            url: '/removeExperiment/'
            success: (data) -> location.href = data.redirect
            error: @_displayError
          )
        )
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


jQuery(document).ready( ->
  new Handlers(root.experimentData, root.reportsList, root.activeExperiment)
)
