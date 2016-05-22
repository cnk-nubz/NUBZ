root = exports ? this
root.cachedData = {}
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
  # constructor :: ([Question], [Action], ExperimentData) -> Context
  constructor: (initQuestionsList, initActionsList, @_experimentData) ->
    @_DOM =
      chooseListTitle: ".chooseList .myHeader span"
      chooseListContainer: ".chooseList .middle"
      actionButton: ".chooseList .myFooter button:first-child"
      questionButton: ".chooseList .myFooter button:last-child"
      attachToExperimentContainer: "td:last-child"
      attachToExperimentContainerSign: "td:last-child > i"
      addElementToList: ".chooseList .addElement"
      experimentTitle: ".experimentTitle input"
      saveExperiment: ".manageExperiment button:first-child"
      removeExperiment: ".manageExperiment button:last-child"

    @_questions = new root.Questions(initQuestionsList)
    @_questionsBefore = new root.Questions(@_experimentData?.surveyBefore)
    @_questionsAfter = new root.Questions(@_experimentData?.surveyAfter)
    @_exhibitActions = new root.Actions(@_experimentData?.exhibitActions)
    @_breakActions = new root.Actions(@_experimentData?.breakActions)
    @_actions = new root.Actions(initActionsList)

    @_chooseQuestionRow = new root.ChooseQuestionRow()
    @_chooseActionRow = new root.ChooseActionRow()
    @_experimentQuestionRow = new root.ExperimentQuestionRow()
    @_experimentActionRow = new root.ExperimentActionRow()
    @_setHandlers()

  _setHandlers: =>
    @_setLists()
    @_setActionButtonClickHandler()
    @_setQuestionButtonClickHandler()
    @_setSaveExperimentHandler()
    @_setRemoveExperimentHandler()
    @_setDefaultState()

  _setLists: =>
    questionsBeforeDOM = @_questionsBefore.getAllElementsAsDOM(@_experimentQuestionRow)
    questionsAfterDOM = @_questionsAfter.getAllElementsAsDOM(@_experimentQuestionRow)
    exhibitActionsDOM = @_exhibitActions.getAllElementsAsDOM(@_experimentActionRow)
    breakActionsDOM = @_breakActions.getAllElementsAsDOM(@_experimentActionRow)
    @_questionsBeforeList = new root.QuestionsList('.questionsBefore .middle', 'questionsActionsTable')
    @_questionsBeforeList
      .show()
      .replaceElements(@_prepareExperimentList(questionsBeforeDOM, @_questionsBefore, @_questionsBeforeList))
    @_questionsAfterList = new root.QuestionsList('.questionsAfter .middle', 'questionsActionsTable')
    @_questionsAfterList
      .show()
      .replaceElements(@_prepareExperimentList(questionsAfterDOM, @_questionsAfter, @_questionsAfterList))
    @_exhibitActionsList = new root.ActionsList('.experimentActions .middle', 'questionsActionsTable')
    @_exhibitActionsList
      .show()
      .replaceElements(@_prepareExperimentList(exhibitActionsDOM, @_exhibitActions, @_exhibitActionsList))
    @_breakActionsList = new root.ActionsList('.breakActions .middle', 'questionsActionsTable')
    @_breakActionsList
      .show()
      .replaceElements(@_prepareExperimentList(breakActionsDOM, @_breakActions, @_breakActionsList))
    questionsDOM =  @_prepareQuestionsList()
    @_questionsList = new root.QuestionsList(".chooseList .middle", 'questionsActionsTable', questionsDOM)
    actionsDOM = @_prepareActionsList()
    @_actionsList = new root.ActionsList(".chooseList .middle", 'questionsActionsTable', actionsDOM)
    return


  # _prepareExperimentList :: (DocumentFragment, root.ExperimentData, root.ListView) -> DocumentFragment
  _prepareExperimentList: (elementsDOM, dataList, contextList) ->
    [].forEach.call(elementsDOM.querySelectorAll("tr"), (element) ->
      viewId = element.data
      element.querySelector("td:first-child")
        .addEventListener("click", ->
          dataList.getFilledDialog(viewId, readonly: true).show()
        )
      element.querySelector("td:last-child")
        .addEventListener("click", ->
          contextList.removeElement(viewId)
          jQuery(this).parents("tr").remove()
        )
    )
    elementsDOM


  # _prepareQuestionsList :: () -> DocumentFragment
  _prepareQuestionsList: =>
    # tl;dr:
    # get all questions as DOM elements
    # attach listeners:
    # 1) first column click: show corresponding dialog
    # 2) last column click: show popover -> call _setAddToExperimentHandler
    questionsDOM = @_questions.getAllElementsAsDOM(@_chooseQuestionRow)
    questionsRows = questionsDOM.querySelectorAll("tr")
    [].forEach.call(questionsRows, (element) =>
      viewId = element.data
      element.querySelector("td:first-child")
        .addEventListener("click", =>
          @_questions.getFilledDialog(viewId, readonly: true).show()
        )
      # contextList has entries corresponding to each popover button
      contextList = [
        @_questionsBeforeList,
        @_questionsAfterList
      ]
      @_setAddToExperimentHandler(element, viewId, contextList, @_questions, @_experimentQuestionRow)
    )
    questionsDOM


  # _prepareActionsList :: () -> DocumentFragment
  _prepareActionsList: =>
    # tl;dr look at _createQuestionsList, it's similiar
    actionsDOM = @_actions.getAllElementsAsDOM(@_chooseActionRow)
    actionsRows = actionsDOM.querySelectorAll("tr")
    [].forEach.call(actionsRows, (element) =>
      viewId = element.data
      element.querySelector("div")
        .addEventListener("click", =>
          @_actions.getFilledDialog(viewId, readonly: true).show()
        )
      contextList = [
        @_exhibitActionsList,
        @_breakActionsList
      ]
      @_setAddToExperimentHandler(element, viewId, contextList, @_actions, @_experimentActionRow)
    )
    actionsDOM

  ###
  # type RowFactory =
  #   root.ExperimentActionRow
  # | root.ExperimentQuestionRow
  ###
  # _setAddToExperimentHandler ::
  #   (jQueryObject, String, [root.ListView], root.ExperimentData, RowFactory) -> undefined
  _setAddToExperimentHandler: (obj, id, contextList, dataList, newElementRowFactory) =>
    # Attach listeners when popover is shown :
    # 1 and 2)
    #    Every button has corresponsing list on contextList
    #    Clicking on some button creates new element based on
    #    id, dataList and newElementRowFactory
    #    And attaches some listeners to it:
    #    1) first column click: show corresponsing dialog
    #    2) last column click: remove element from list
    instance = this
    dataContentButtons = null
    jQuery("td:last-child", obj).click((e) ->
      if not jQuery(e.target).is("button")
        jQuery("i", this).focus()
    )
    jQuery("td:last-child > i", obj)
      .popover()
      .on('shown.bs.popover', ->
        jQuery(".popover-content button").each((index) ->
          if contextList[index].isElementOnList(id)
            jQuery(this).addClass("disabled")
          else
            jQuery(this).mousedown( ->
              newElement = dataList.getElementAsDOM(id, newElementRowFactory)
              jQuery("i", newElement).parent().click( ->
                contextList[index].removeElement(id)
                jQuery(this).parents("tr").remove()
              )
              jQuery("td:first-child", newElement).click( ->
                dataList.getFilledDialog(id, readonly: true).show()
              )
              contextList[index].addElement(newElement)
            )
        )
      )
      .each((index) ->
        jQuery(this).attr(tabindex: index)
        # add bootstrap class to every popover button
        dataContent = jQuery(this).attr("data-content")
        dataContentButtons = jQuery(dataContent).each( ->
          jQuery(this).addClass("btn btn-default btn-xs")
        )
      )
      .attr("data-content", "#{dataContentButtons[0].outerHTML}#{dataContentButtons[1].outerHTML}")
    return


  # _setDefaultState :: () -> undefined
  _setDefaultState: =>
    if @_experimentData?
      jQuery(@_DOM.experimentTitle).val(@_experimentData.name)
    else
      jQuery(@_DOM.removeExperiment).remove()
    @_questionsList.show()
    jQuery(@_DOM.chooseListTitle).text("Pytania")
    jQuery(@_DOM.questionButton).addClass("active")
    jQuery(@_DOM.addElementToList).click( => @_addNewQuestion())
    @_questionsBeforeList.show()
    @_questionsAfterList.show()
    @_exhibitActionsList.show()
    @_breakActionsList.show()
    return


  # _setQuestionButtonClickHandler :: () -> undefined
  _setQuestionButtonClickHandler: =>
    instance = this
    jQuery(@_DOM.questionButton).click( ->
      jQuery(this).blur()
      if jQuery(this).hasClass("active")
        return
      jQuery(this).addClass("active")
      jQuery(instance._DOM.actionButton).removeClass("active")
      jQuery(instance._DOM.chooseListTitle).text("Pytania")
      instance._actionsList.hide()
      instance._questionsList.show()
      jQuery(instance._DOM.addElementToList).off("click").click( ->
        instance._addNewQuestion()
      )
    )
    return


  # _setActionButtonClickHandler :: () -> undefined
  _setActionButtonClickHandler: =>
    instance = this
    jQuery(@_DOM.actionButton).click( ->
      jQuery(this).blur()
      if jQuery(this).hasClass("active")
        return
      jQuery(this).addClass("active")
      jQuery(instance._DOM.questionButton).removeClass("active")
      jQuery(instance._DOM.chooseListTitle).text("Akcje")
      instance._questionsList.hide()
      instance._actionsList.show()
      jQuery(instance._DOM.addElementToList).off("click").click( ->
        (new root.ActionDialog(root.structures.dialog.action))
          .on('save', instance._newEntryRequest("/action/", instance._actionsChanged))
          .show()
      )
    )
    return


  # _actionsChanged :: ({actionsList :: [Action]}) -> undefined
  _actionsChanged: (data) =>
    @_actions.setElements(data.actionsList)
    toAdd = @_prepareActionsList()
    @_actionsList.replaceElements(toAdd)
    return


  # _setSaveExperimentHandler :: () -> undefined
  _setSaveExperimentHandler: =>
    jQuery(@_DOM.saveExperiment).click( =>
      if @_isExperimentReady()
        @_saveExperimentRequest()
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

  # _isExperimentReady :: () -> Boolean
  _isExperimentReady: =>
    experimentTitle = jQuery(@_DOM.experimentTitle)
    titleText = jQuery.trim(experimentTitle.val())
    experimentTitle.val(titleText)
    if titleText.length is 0
      experimentTitle.tooltip('show')
      experimentTitle.parent().addClass("has-error")
      experimentTitle.one('focus', ->
        jQuery(this).parent().removeClass("has-error")
        jQuery(this).tooltip('destroy')
      )
      jQuery("#experiment").scrollTop(0)
      return false
    true


  # _saveExperimentRequest :: () -> undefined
  _saveExperimentRequest: =>
    dataToSend = {
      experimentId: @_experimentData.experimentId if @_experimentData?
      name: jQuery(@_DOM.experimentTitle).val()
      surveyBefore: @_questionsBeforeList.getAllElements()
      exhibitActions: @_exhibitActionsList.getAllElements()
      breakActions: @_breakActionsList.getAllElements()
      surveyAfter: @_questionsAfterList.getAllElements()
    }
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: if @_experimentData? then "PUT" else "POST"
      dataType: 'json'
      context: this
      data: (jsonData: JSON.stringify(dataToSend))
      url: '/experiment/'
      statusCode:
        403: @_showNameDuplicatedError
        500: @_displayError
      success: (data) -> location.href = data.redirect
    )
    return


  # _showNameDuplicatedError :: jqXHR -> undefined
  _showNameDuplicatedError: (obj) =>
    message = obj.responseText
    experimentTitle = jQuery(@_DOM.experimentTitle)
    oldTitle = experimentTitle.attr("title")
    experimentTitle
      .attr("title", message)
      .tooltip('show')
      .one('focus', ->
        jQuery(this).attr("title", oldTitle)
        jQuery(this).parent().removeClass("has-error")
        jQuery(this).tooltip('destroy')
      )
      .parent().addClass("has-error")
    jQuery('#experiment').scrollTop(0)
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


  # _questionsChanged :: ({questionsList :: [Question]}) -> undefined
  _questionsChanged: (data) =>
    @_questions.setElements(data.questionsList)
    toAdd = @_prepareQuestionsList()
    @_questionsList.replaceElements(toAdd)
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
  new Handlers(root.questionsList, root.actionsList, root.experimentData)
)
