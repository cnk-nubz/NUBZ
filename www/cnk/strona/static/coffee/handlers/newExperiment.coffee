root = exports ? this
class Handlers
  constructor: (initQuestionsList, initActionsList) ->
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
    @_actions = new root.Actions(initActionsList)

    @_questionsList = new root.QuestionsList(".chooseList .middle", new root.ChooseQuestionRow())
    @_actionsList = new root.ActionsList(".chooseList .middle", new root.ChooseActionRow())
    @_questionsBeforeList = new root.QuestionsList(".questionsBefore .middle", new root.ExperimentQuestionRow())
    @_questionsAfterList = new root.QuestionsList(".questionsAfter .middle", new root.ExperimentQuestionRow())
    @_experimentActionsList = new root.ActionsList(".experimentActions .middle", new root.ExperimentActionRow())
    @_breakActionsList = new root.ActionsList(".breakActions .middle", new root.ExperimentActionRow())
    @_setHandlers()

  _setHandlers: =>
    @_setActionButtonClickHandler()
    @_setQuestionButtonClickHandler()
    @_setListsHandlers()
    @_setSaveExperimentHandler()

  _setListsHandlers: =>
    #default state
    @_setListsElementAddedHandler()
    @_setListsInitHandler()
    @_setNameClickedHandler()
    @_setRemoveFromExperimentHandler()
    return

  _setListsInitHandler: =>
    instance = this
    @_questionsList.addElements(@_questions.listFormat())
    jQuery(@_DOM.questionButton).click()
    @_actionsList.addElements(@_actions.listFormat())
    @_questionsBeforeList.show()
    @_questionsAfterList.show()
    @_experimentActionsList.show()
    @_breakActionsList.show()
    return

  _setNameClickedHandler: =>
    @_questionsList.on("nameClicked", (id) => @_questions.showDialog(id, true))
    @_actionsList.on("nameClicked", (id) => @_actions.showDialog(id, true))
    @_questionsBeforeList.on("nameClicked", (id) => @_questions.showDialog(id, true))
    @_questionsAfterList.on("nameClicked", (id) => @_questions.showDialog(id, true))
    @_experimentActionsList.on("nameClicked", (id) => @_actions.showDialog(id, true))
    @_breakActionsList.on("nameClicked", (id) => @_actions.showDialog(id, true))
    return

  _setRemoveFromExperimentHandler: =>
    @_questionsBeforeList.on("removeFromExperiment", (id, pos) => @_questionsBeforeList.removeElement(pos))
    @_questionsAfterList.on("removeFromExperiment", (id, pos) => @_questionsAfterList.removeElement(pos))
    @_experimentActionsList.on("removeFromExperiment", (id, pos) => @_experimentActionsList.removeElement(pos))
    @_breakActionsList.on("removeFromExperiment", (id, pos) => @_breakActionsList.removeElement(pos))
    return

  _setListsElementAddedHandler: =>
    @_questionsList.on("elementAdded", (obj) =>
      contextList = [
        @_questionsBeforeList
        @_questionsAfterList
      ]
      @_setAddToExperimentHandler(obj, contextList)
    )
    @_actionsList.on("elementAdded", (obj) =>
      contextList = [
        @_experimentActionsList
        @_breakActionsList
      ]
      @_setAddToExperimentHandler(obj, contextList)
    )
    return

  _setQuestionButtonClickHandler: =>
    instance = this
    jQuery(@_DOM.questionButton).click( ->
      jQuery(this).blur()
      return if jQuery(this).hasClass("active")
      jQuery(this).addClass("active")
      jQuery(instance._DOM.actionButton).removeClass("active")
      jQuery(instance._DOM.chooseListTitle).text("Pytania")
      instance._questionsList.show()
      jQuery(instance._DOM.addElementToList).off("click").click( =>
        instance._addNewQuestion()
      )
    )
    return

  _setActionButtonClickHandler: =>
    instance = this
    jQuery(@_DOM.actionButton).click( ->
      jQuery(this).blur()
      return if jQuery(this).hasClass("active")
      jQuery(this).addClass("active")
      jQuery(instance._DOM.questionButton).removeClass("active")
      jQuery(instance._DOM.chooseListTitle).text("Akcje")
      instance._actionsList.show()
      actionSave = instance._newEntryRequest("/createAction/", instance._createNewAction)
      actionDialog = new root.ActionDialog("getActionDialog/", actionSave)
      jQuery(instance._DOM.addElementToList).off("click").click( =>
        actionDialog.show()
      )
    )
    return

  _setSaveExperimentHandler: =>
    jQuery(@_DOM.saveExperiment).click( =>
      experimentTitle = jQuery(@_DOM.experimentTitle)
      titleText = experimentTitle.val()
      experimentTitle.val(jQuery.trim(titleText))
      if not titleText.match root.inputRegex
        experimentTitle.tooltip('show')
        experimentTitle.parent().addClass("has-error")
        experimentTitle.one('focus', ->
          jQuery(this).parent().removeClass("has-error")
          jQuery(this).tooltip('destroy')
        )
        jQuery("#experiment").scrollTop(0)
        return
      # otherwise collect data and try to save them
      questionsBefore = @_questionsBeforeList.getAllElementsId()
      questionsAfter = @_questionsAfterList.getAllElementsId()
      experimentActions = @_experimentActionsList.getAllElementsId()
      breakActions = @_breakActionsList.getAllElementsId()
      dataToSend = {
        questionsBefore: questionsBefore
        questionsAfter: questionsAfter
        experimentActions: experimentActions
        breakActions: breakActions
      }
      jQuery.ajaxSetup(
        headers: { "X-CSRFToken": getCookie("csrftoken") }
      )
      jQuery.ajax(
        type: 'POST'
        dataType: 'json'
        context: this
        data: (jsonData: JSON.stringify(dataToSend))
        url: '/createExperiment/'
        success: (data) -> @_displayError(data.message) unless data.success
      )
    )

  _setAddToExperimentHandler: (obj, contextList) ->
    dataContentButtons = null
    jQuery(@_DOM.attachToExperimentContainer, obj).click((e) ->
      if not jQuery(e.target).is("button")
        jQuery("i", this).focus()
    )
    jQuery(@_DOM.attachToExperimentContainerSign, obj)
      .popover()
      .on('shown.bs.popover', ->
        jQuery(".popover-content button").each((index) ->
          rowData = jQuery(this).parents("tr").data("rowData")
          elementId = Object.keys(rowData)[0]
          if contextList[index].isElementOnList(elementId)
            jQuery(this).addClass("disabled")
          else
            jQuery(this).mousedown( -> contextList[index].addElements(rowData).showLast())
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

  _addNewQuestion: =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      dataType: 'json'
      context: this
      url: '/getChooseQuestionTypeDialog/'
      success: (data) => @_showChooseQuestionTypeDialog(data.html)
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

  # handlers for dialog to select question type
  _setChooseQuestionTypeHandlers: (dialog) =>
    # Long names are long. Are you gazing into the abyss? Because it's gazing back at us.
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
    toAdd = @_questions.listFormat(data.questionsList)
    @_questionsList.replaceElements(toAdd).show()
    return

  _createNewAction: (data) =>
    if not data.success
      @_displayError(data.message)
      return
    @_actions.setElements(data.actionsList)
    toAdd = @_actions.listFormat(data.actionsList)
    @_actionsList.replaceElements(toAdd).show()
    return

  _displayError: (message) =>
    BootstrapDialog.show(
      message: message
      title: 'Wystąpił błąd'
      type: BootstrapDialog.TYPE_DANGER
    )

jQuery(document).ready( ->
  new Handlers(root.questionsList, root.actionsList)
)
