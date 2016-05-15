root = exports ? this
root.cachedData = {}
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
          .on('delete', @_deleteAction)
          .show()
        )
    )
    actionsDOM


  # _setHandler :: () -> undefined
  _setHandlers: =>
    jQuery("#questionList .addElement").on('click', @_addNewQuestion)
    new root.ActionDialog() #to load to cache TODO
    jQuery("#actionList .addElement").click( =>
      new root.ActionDialog()
        .on('save', @_newEntryRequest("/action/", @_actionsChanged))
        .show()
    )
    return


  # _addNewQuestion :: () -> undefined
  _addNewQuestion: =>
    jQuery.getJSON('getHTML?name=chooseQuestionTypeDialog', null, (data) =>
      @_showChooseQuestionType(data.html)
    )
    return


  # _showChooseQuestionType :: String -> undefined
  _showChooseQuestionType: (html) =>
    BootstrapDialog.show(
      message: html
      title: 'Wybierz typ pytania'
      onshown: (dialog) =>
        @_setQuestionsHandlers(dialog)
    )
    return


  # _setQuestionsHandlers :: BootstrapDialog -> undefined
  _setQuestionsHandlers: (dialog) =>
    saveHandler = @_newEntryRequest("/question/", @_questionsChanged)
    simpleQuestionDialog = new root.SimpleQuestionDialog()
      .on('save', saveHandler)
    sortQuestionDialog = new root.SortQuestionDialog()
      .on('save', saveHandler)
    multipleChoiceQuestionDialog = new root.MultipleChoiceQuestionDialog()
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


  # _deleteAction :: Int -> undefined
  _deleteAction: (actionId) =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      method: 'DELETE'
      dataType: 'json'
      data: (jsonData: JSON.stringify(id: actionId))
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
