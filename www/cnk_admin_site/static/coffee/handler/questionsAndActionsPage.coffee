root = exports ? this
class Handlers
  # constructor :: ([Question], [Action]) -> Context
  constructor: (initQuestionsList, initActionsList) ->
    @_questions = new root.Questions(initQuestionsList)
    @_actions = new root.Actions(initActionsList)
    @_showQuestionRow = new root.ShowQuestionRow()
    @_showActionRow = new root.ShowActionRow()
    @_setDefaultState()
    @_setHandlers()


  # _setDefaultState () -> undefined
  _setDefaultState: =>
    questionsDOM = @_prepareQuestionsList()
    actionsDOM = @_prepareActionsList()
    @_questionsList = new root.QuestionsList('#questionList .middle', "questionsActionsTable", questionsDOM)
    @_actionsList = new root.ActionsList('#actionList .middle', "questionsActionsTable", actionsDOM)
    @_questionsList.show()
    @_actionsList.show()
    return


  # _prepareQuestionsList :: () -> DocumentFragment
  _prepareQuestionsList: =>
    questionsDOM = @_questions.getAllElementsAsDOM(@_showQuestionRow)
    [].forEach.call(questionsDOM.querySelectorAll("tr"), (element) =>
      viewId = element.data
      element.querySelector("td:first-child")
        .addEventListener("click", =>
          @_questions.getFilledDialog(viewId,
            readonly: true
            deletable: true
          ).off('delete')
          .on('delete', @_deleteQuestion)
          .show()
        )
    )
    questionsDOM


  # _prepareActionsList :: () -> DocumentFragment
  _prepareActionsList: =>
    actionsDOM = @_actions.getAllElementsAsDOM(@_showActionRow)
    [].forEach.call(actionsDOM.querySelectorAll("tr"), (element) =>
      viewId = element.data
      element.querySelector("td:first-child")
        .addEventListener("click", =>
          @_actions.getFilledDialog(viewId,
            readonly: true
            deletable: true
          ).off('delete')
          .on('delete', @_removeAction)
          .show()
        )
    )
    actionsDOM


  # _setHandler :: () -> undefined
  _setHandlers: =>
    jQuery("#questionList .addElement").on('click', @_addNewQuestion)
    jQuery("#actionList .addElement").click( =>
      new root.ActionDialog(root.structures.dialog.action)
        .on('save', @_newEntryRequest("/action/", @_actionsChanged))
        .show()
    )
    return


  # _addNewQuestion :: () -> undefined
  _addNewQuestion: =>
    BootstrapDialog.show(
      message: root.structures.HTML.chooseQuestionType
      title: 'Wybierz typ pytania'
      onshown: @_setQuestionsHandlers
    )
    return


  # _setQuestionsHandlers :: BootstrapDialog -> undefined
  _setQuestionsHandlers: (dialog) =>
    saveHandler = @_newEntryRequest("/question/", @_questionsChanged)
    simpleQuestionDialog = new root.SimpleQuestionDialog(
      root.structures.dialog.question.simple)
      .on('save', saveHandler)
    sortQuestionDialog = new root.SortQuestionDialog(
      root.structures.dialog.question.sort)
      .on('save', saveHandler)
    multipleChoiceQuestionDialog = new root.MultipleChoiceQuestionDialog(
      root.structures.dialog.question.multipleChoice)
      .on('save', saveHandler)
    jQuery("#dialog button").click( -> dialog.close())
    jQuery("#simpleQuestion").click( -> simpleQuestionDialog.show())
    jQuery("#sortQuestion").click( -> sortQuestionDialog.show())
    jQuery("#multipleChoiceQuestion").click( -> multipleChoiceQuestionDialog.show())
    return


  ###
  # type RequestData =
  #   QuestionData
  # | ActionData
  ###
  ###
  # type ResponseData = {
  #   questionsList :: [Question],
  # }
  # | {
  #   actionsList   :: [Action],
  # }
  # | jqXHR
  ###
  # _newEntryRequest :: (String, (ResponseData -> undefined))
  #                     -> (RequestData, BootstrapDialog) -> undefined
  _newEntryRequest: (url, callback) =>
    (data, dialog) =>
      jQuery.ajaxSetup(
        headers: { "X-CSRFToken": getCookie("csrftoken") }
      )
      jQuery.ajax(
        type: 'POST'
        dataType: 'json'
        data: (jsonData: JSON.stringify(data))
        url: url
        statusCode:
          403: -> dialog.showNameDuplicatedError()
          500: @_displayError
        success: (recvData) ->
          callback(recvData)
          dialog.close()
      )
      return


  # _removeAction :: Int -> undefined
  _removeAction: (actionId) =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      method: 'DELETE'
      dataType: 'json'
      data: (jsonData: JSON.stringify(actionId: actionId))
      url: '/action/'
      error: @_displayError
      success: @_actionsChanged
    )
    return


  # _deleteQuestion :: Int -> undefined
  _deleteQuestion: (questionId, type) =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    toSend =
      jsonData: JSON.stringify(
        questionId: questionId
        type: type
      )
    jQuery.ajax(
      method: 'DELETE'
      dataType: 'json'
      data: toSend
      url: '/question/'
      error: @_displayError
      success: @_questionsChanged
    )
    return

  # _questionsChanged :: ({questionsList :: [Question]}) -> undefined
  _questionsChanged: (data) =>
    @_questions.setElements(data.questionsList)
    toAdd = @_prepareQuestionsList()
    @_questionsList.replaceElements(toAdd)
    return


  # _actionsChanged :: ({actionsList :: [Action]}) -> undefined
  _actionsChanged: (data) =>
    @_actions.setElements(data.actionsList)
    toAdd = @_prepareActionsList()
    @_actionsList.replaceElements(toAdd)
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
  new Handlers(root.questionsList, root.actionsList)
)
