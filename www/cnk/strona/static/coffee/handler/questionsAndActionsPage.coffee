root = exports ? this
class Handlers
  constructor: (initQuestionsList, initActionsList) ->
    @_questions = new root.Questions(initQuestionsList)
    @_actions = new root.Actions(initActionsList)
    @_showQuestionRow = new root.ShowQuestionRow()
    @_showActionRow = new root.ShowActionRow()
    @_setDefaultState()
    @_setHandlers()

  _setDefaultState: =>
    questionsDOM = @_prepareQuestionsList()
    actionsDOM = @_prepareActionsList()
    @_questionsList = new root.QuestionsList('#questionList .middle', "questionsActionsTable", questionsDOM)
    @_actionsList = new root.ActionsList('#actionList .middle', "questionsActionsTable", actionsDOM)
    @_questionsList.show()
    @_actionsList.show()

  _prepareQuestionsList: =>
    questionsDOM = @_questions.getAllElementsAsDOM(@_showQuestionRow)
    [].forEach.call(questionsDOM.querySelectorAll("tr"), (element) =>
      viewId = element.data
      element.querySelector("td:first-child")
        .addEventListener("click", =>
          @_questions.showDialog(viewId, true)
        )
    )
    questionsDOM

  _prepareActionsList: =>
    actionsDOM = @_actions.getAllElementsAsDOM(@_showActionRow)
    [].forEach.call(actionsDOM.querySelectorAll("tr"), (element) =>
      viewId = element.data
      element.querySelector("td:first-child")
        .addEventListener("click", =>
          @_actions.showDialog(viewId, true)
        )
    )
    actionsDOM

  _setHandlers: =>
    jQuery("#questionList .addElement").on('click', @_addNewQuestion)
    actionSave = @_newEntryRequest("/createAction/", @_createNewAction)
    actionDialog = new root.ActionDialog("getActionDialog/", actionSave)
    jQuery("#actionList .addElement").click( ->
      actionDialog.show()
    )
    new root.ActionDialog("getActionDialog/", "#actionList .addElement", @_actionCreatedHandler)
    return

  _addNewQuestion: =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      dataType: 'json'
      context: this
      url: '/getChooseQuestionTypeDialog/'
      success: (data) =>
        @_showChooseQuestionType(data.html)
    )
    return

  _showChooseQuestionType: (html) =>
    BootstrapDialog.show(
      message: html
      title: 'Wybierz typ pytania'
      closable: false
      buttons: [
        label: 'Anuluj'
        action: (dialog) ->
          dialog.close()
      ]
      onshown: (dialog) =>
         @_setQuestionsHandlers(dialog)
    )
    return

  _setQuestionsHandlers: (dialog) =>
    simpleQuestionSave = @_newEntryRequest("/createSimpleQuestion/", @_createNewQuestion)
    simpleQuestionDialog = new root.SimpleQuestionDialog('getSimpleQuestionDialog/', simpleQuestionSave)
    sortQuestionSave = @_newEntryRequest("/createSortQuestion/", @_createNewQuestion)
    sortQuestionDialog = new root.SortQuestionDialog('getSortQuestionDialog/', sortQuestionSave)
    multipleChoiceSave = @_newEntryRequest("/createMultipleChoiceQuestion/", @_createNewQuestion)
    multipleChoiceQuestionDialog = new root.MultipleChoiceQuestionDialog('getMultipleChoiceQuestionDialog/', multipleChoiceSave)
    jQuery("#dialog button").click( -> dialog.close())
    jQuery("#simpleQuestion").click( -> simpleQuestionDialog.show())
    jQuery("#sortQuestion").click( -> sortQuestionDialog.show())
    jQuery("#multipleChoiceQuestion").click( -> multipleChoiceQuestionDialog.show())
    jQuery("#dialog button").click( -> dialog.close())
    return

  _newEntryRequest: (url, callback) =>
    (data) =>
      jQuery.ajaxSetup(
        headers: { "X-CSRFToken": getCookie("csrftoken") }
      )
      jQuery.ajax(
        type: 'POST'
        dataType: 'json'
        data: (jsonData: JSON.stringify(data))
        url: url
        success: (data) =>
          callback(data)
      )
      return

  _createNewQuestion: (data) =>
    if not data.success
      @_displayError(data.message)
      return
    @_questions.setElements(data.questionsList)
    toAdd = @_prepareQuestionsList()
    @_questionsList.replaceElements(toAdd)
    return

  _createNewAction: (data) =>
    if not data.success
      @_displayError(data.message)
      return
    @_actions.setElements(data.actionsList)
    toAdd = @_prepareActionsList()
    @_actionsList.replaceElements(toAdd)
    return

  _displayError: (message) =>
    BootstrapDialog.show(
      message: message
      title: 'Wystąpił błąd'
      type: BootstrapDialog.TYPE_DANGER
    )
    return

jQuery(document).ready( ->
  new Handlers(root.questionsList, root.actionsList)
)
