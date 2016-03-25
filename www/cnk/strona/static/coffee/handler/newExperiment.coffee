root = exports ? this
class Handlers
  constructor: (initQuestionsList, initActionsList, @_experimentData, @_isExperimentReadonly) ->
    @_DOM =
      chooseListTitle: ".chooseList .myHeader span"
      chooseListContainer: ".chooseList .middle"
      actionButton: ".chooseList .myFooter button:first-child"
      questionButton: ".chooseList .myFooter button:last-child"
      attachToExperimentContainer: "td:last-child"
      attachToExperimentContainerSign: "td:last-child > i"
      addElementToList: ".chooseList .addElement"
      experimentTitle: ".experimentTitle input"
      saveExperiment: ".saveExperiment button"

    @_questions = new root.Questions(initQuestionsList)
    @_questionsBefore = new root.Questions(@_experimentData?.questionsBefore)
    @_questionsAfter = new root.Questions(@_experimentData?.questionsAfter)
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

  _prepareExperimentList: (elementsDOM, dataList, contextList) =>
    [].forEach.call(elementsDOM.querySelectorAll("tr"), (element) =>
      viewId = element.data
      element.querySelector("td:first-child")
        .addEventListener("click", ->
          dataList.showDialog(viewId, true)
        )
      if not @_isExperimentReadonly
        element.querySelector("td:last-child")
          .addEventListener("click", ->
            contextList.removeElement(viewId)
            jQuery(this).parents("tr").remove()
          )
    )
    elementsDOM

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
          @_questions.showDialog(viewId, true)
        )
      # contextList has entries corresponding to each popover button
      contextList = [
        @_questionsBeforeList,
        @_questionsAfterList
      ]
      @_setAddToExperimentHandler(element, viewId, contextList, @_questions, @_experimentQuestionRow)
    )
    questionsDOM

  _prepareActionsList: =>
    # tl;dr look at _createQuestionsList, it's similiar
    actionsDOM = @_actions.getAllElementsAsDOM(@_chooseActionRow)
    actionsRows = actionsDOM.querySelectorAll("tr")
    [].forEach.call(actionsRows, (element) =>
      viewId = element.data
      element.querySelector("div")
        .addEventListener("click", =>
          @_actions.showDialog(viewId, true)
        )
      contextList = [
        @_exhibitActionsList,
        @_breakActionsList
      ]
      @_setAddToExperimentHandler(element, viewId, contextList, @_actions, @_experimentActionRow)
    )
    actionsDOM

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
                dataList.showDialog(id, true)
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

  _setDefaultState: =>
    if not @_isExperimentReadonly
      @_questionsList.show()
      jQuery(@_DOM.chooseListTitle).text("Pytania")
      jQuery(@_DOM.questionButton).addClass("active")
      jQuery(@_DOM.addElementToList).click( => @_addNewQuestion())
    if @_experimentData?
      jQuery(@_DOM.experimentTitle).val(@_experimentData.name)
    @_questionsBeforeList.show()
    @_questionsAfterList.show()
    @_exhibitActionsList.show()
    @_breakActionsList.show()
    return

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
      actionSave = instance._newEntryRequest("/createAction/", instance._createNewAction)
      actionDialog = new root.ActionDialog("getHTML?name=actionDialog", actionSave)
      jQuery(instance._DOM.addElementToList).off("click").click( ->
        actionDialog.show()
      )
    )
    return

  _createNewAction: (data) =>
    if not data.success
      @_displayError(data.message)
      return
    @_actions.setElements(data.actionsList)
    toAdd = @_prepareActionsList()
    @_actionsList.replaceElements(toAdd)
    return

  _setSaveExperimentHandler: =>
    jQuery(@_DOM.saveExperiment).click( =>
      if @_isExperimentReady()
        @_saveExperimentRequest()
    )

  _isExperimentReady: =>
    experimentTitle = jQuery(@_DOM.experimentTitle)
    titleText = experimentTitle.val()
    experimentTitle.val(jQuery.trim(titleText))
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
      type: 'POST'
      dataType: 'json'
      context: this
      data: (jsonData: JSON.stringify(dataToSend))
      url: '/saveExperiment/'
      success: (data) ->
        if not data.success
          @_displayError(data.message)
        else
          location.href = '/badania'
    )
    return

  _addNewQuestion: =>
    jQuery.getJSON('getHTML?name=chooseQuestionTypeDialog', null, (data) =>
      @_showChooseQuestionTypeDialog(data.html)
    )
    return

  _showChooseQuestionTypeDialog: (html) =>
    BootstrapDialog.show(
      message: html
      title: 'Wybierz typ pytania'
      closable: false
      buttons: [
        label: 'Anuluj'
        action: (dialog) -> dialog.close()
      ]
      onshown: (dialog) => @_setChooseQuestionTypeHandlers(dialog)
    )
    return

  _setChooseQuestionTypeHandlers: (dialog) =>
    # Long names are long. Are you gazing into the abyss? Because it's gazing back at us.
    simpleQuestionSave = @_newEntryRequest("/createSimpleQuestion/", @_createNewQuestion)
    simpleQuestionDialog = new root.SimpleQuestionDialog('getHTML?name=simpleQuestionDialog', simpleQuestionSave)
    sortQuestionSave = @_newEntryRequest("/createSortQuestion/", @_createNewQuestion)
    sortQuestionDialog = new root.SortQuestionDialog('getHTML?name=sortQuestionDialog', sortQuestionSave)
    multipleChoiceSave = @_newEntryRequest("/createMultipleChoiceQuestion/", @_createNewQuestion)
    multipleChoiceQuestionDialog = new root.MultipleChoiceQuestionDialog('getHTML?name=multipleChoiceQuestionDialog', multipleChoiceSave)
    jQuery("#dialog button").click( -> dialog.close())
    jQuery("#simpleQuestion").click( -> simpleQuestionDialog.show())
    jQuery("#sortQuestion").click( -> sortQuestionDialog.show())
    jQuery("#multipleChoiceQuestion").click( -> multipleChoiceQuestionDialog.show())
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

  _displayError: (message) =>
    BootstrapDialog.show(
      message: message
      title: 'Wystąpił błąd'
      type: BootstrapDialog.TYPE_DANGER
    )

jQuery(document).ready( ->
  new Handlers(root.questionsList, root.actionsList, root.experimentData, root.isExperimentReadonly)
)
