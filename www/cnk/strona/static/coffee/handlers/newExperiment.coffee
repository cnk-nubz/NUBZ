root = exports ? this

class Questions
  constructor: (@_list = {}) ->
    @_init()

  _init: =>
    @_simpleQuestionDialog = new root.SimpleQuestionDialog('getSimpleQuestionDialog/')
    @_multipleChoiceQuestionDialog = new root.MultipleChoiceQuestionDialog('getMultipleChoiceQuestionDialog/')
    @_sortQuestionDialog = new root.SortQuestionDialog('getSortQuestionDialog/')
    @_questionType =
      0:
        name: 'otwarte'
        dialog: @_simpleQuestionDialog
      1:
        name: 'zamknięte'
        dialog: @_multipleChoiceQuestionDialog
      2:
        name: 'sortowanie'
        dialog: @_sortQuestionDialog
  addElements: (elements) => @_list[element.questionId] = element for element in elements

  removeElement: (id) => delete @_list[id] if @_list[id]?

  parseAllForTwoColumns: =>
    data = {}
    data[key] = @_parseForTwoColumns key for key in Object.keys(@_list)
    data

  parseElementForTwoColumns: (id) =>
    data = {}
    data[id] = @_parseForTwoColumns(id)
    data

  getElement: (id) => @_list[id]

  _getElementNameType: (id) => @_questionType[@_list[id].type].name

  _parseForTwoColumns: (id) => { first: @_list[id].name }

  parseAllForThreeColumns: =>
    data = {}
    data[key] = @_parseForThreeColumns key for key in Object.keys(@_list)
    data

  parseElementForThreeColumns: (id) =>
    data = {}
    data[id] = @_parseForThreeColumns(id)
    data

  _parseForThreeColumns: (id) => { first: @_list[id].name, second: @_getElementNameType(id) }

  showDialog: (id, readonly) =>
    dialog = @_questionType[@_list[id].type].dialog
    dialog.readonly = if readonly? and readonly then true else false
    @_questionType[@_list[id].type].dialog.bindData(@_list[id]).show()

class Actions
  constructor: (@_list = {}) ->
    @_init()

  _init: =>
    @_actionDialog = new root.ActionDialog('getActionDialog/')

  addElements: (elements) => @_list[element.actionId] = element for element in elements

  removeElement: (id) => delete @_list[id] if @_list[id]?

  parseAllForTwoColumns: =>
    data = {}
    data[key] = @_parseForTwoColumns key for key in Object.keys(@_list)
    data

  parseElementForTwoColumns: (id) =>
    data = {}
    data[id] = @_parseForTwoColumns(id)
    data

  getElement: (id) => @_list[id]

  showDialog: (id, readonly) =>
    @_actionDialog.readonly = if readonly? and readonly then true else false
    @_actionDialog.bindData(@_list[id]).show()

  _parseForTwoColumns: (id) => { first: @_list[id].text }

class Handlers
  constructor: ->
    @_DOM =
      header: ".chooseList .myHeader span"
      actionButton: ".chooseList .myFooter button:first-child"
      questionButton: ".chooseList .myFooter button:last-child"
      attachToExperimentContainer: "td:last-child"
      attachToExperimentContainerSign: "td:last-child > i"
      addElement: ".chooseList .addElement"

    # TODO: add questions and actions from server
    @_questions = new Questions()
    @_actions = new Actions()
    @_questionsList = new root.ThreeColumnList(".chooseList .middle", 'plus')
    @_actionsList = new root.TwoColumnList(".chooseList .middle", 'plus')
    @_questionsBeforeList = new root.TwoColumnList(".questionsBefore .middle", 'minus')
    @_questionsAfterList = new root.TwoColumnList(".questionsAfter .middle", 'minus')
    @_experimentActionsList = new root.TwoColumnList(".experimentActions .middle", 'minus')
    @_breakActionsList = new root.TwoColumnList(".breakActions .middle", 'minus')
    @_setHandlers()

  _setHandlers: =>
    @_setListsHandlers()
    @_setActionButtonClickHandler()
    @_setQuestionButtonClickHandler()

  _setListsHandlers: =>
    #default state
    @_setListsInitHandler()
    @_setListsFirstClickHandler()
    @_setListsSecondClickHandler()
    @_setListsElementAddedHandler()
    jQuery(@_DOM.addElement).click( => @_addNewQuestion())
    jQuery(@_DOM.header).text("Pytania")
    return

  _setListsInitHandler: =>
    instance = this
    @_questionsList.on("init", =>
      @_questionsList.replaceElements(@_questions.parseAllForThreeColumns()).show()
      jQuery(@_DOM.questionButton).click()
    ).init()
    @_actionsList.on("init", => @_actionsList.replaceElements(@_actions.parseAllForTwoColumns())).init()
    @_questionsBeforeList.on("init", => @_questionsBeforeList.show()).init()
    @_questionsAfterList.on("init", => @_questionsAfterList.show()).init()
    @_experimentActionsList.on("init", => @_experimentActionsList.show()).init()
    @_breakActionsList.on("init", => @_breakActionsList.show()).init()
    return

  _setListsFirstClickHandler: =>
    @_questionsList.on("firstClicked", (id) => @_questions.showDialog(id))
    @_actionsList.on("firstClicked", (id) => @_actions.showDialog(id))
    @_questionsBeforeList.on("firstClicked", (id) => @_questions.showDialog(id, true))
    @_questionsAfterList.on("firstClicked", (id) => @_questions.showDialog(id, true))
    @_experimentActionsList.on("firstClicked", (id) => @_actions.showDialog(id, true))
    @_breakActionsList.on("firstClicked", (id) => @_actions.showDialog(id, true))
    return

  _setListsSecondClickHandler: =>
    @_questionsBeforeList.on("secondClicked", (id, pos) => @_questionsBeforeList.removeElement(pos))
    @_questionsAfterList.on("secondClicked", (id, pos) => @_questionsAfterList.removeElement(pos))
    @_experimentActionsList.on("secondClicked", (id, pos) => @_experimentActionsList.removeElement(pos))
    @_breakActionsList.on("secondClicked", (id, pos) => @_breakActionsList.removeElement(pos))
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
      jQuery(instance._DOM.header).text("Pytania")
      instance._questionsList.show()
      jQuery(instance._DOM.addElement).off("click").click( =>
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
      jQuery(instance._DOM.header).text("Akcje")
      instance._actionsList.show()
      actionDialog = new root.ActionDialog("getActionDialog/", instance._actionCreatedHandler)
      jQuery(instance._DOM.addElement).off("click").click( =>
        actionDialog.show()
      )
    )
    return

  _setAddToExperimentHandler: (obj, contextList) ->
    dataContentButtons = null
    jQuery(@_DOM.attachToExperimentContainer, obj).click((e) ->
      if not jQuery(e.target).is("button")
        jQuery(this).children("i").focus()
      e.stopPropagation()
    )
    jQuery(@_DOM.attachToExperimentContainerSign, obj)
      .popover()
      .on('shown.bs.popover', ->
        jQuery(".popover-content button").each((index) ->
          jQuery(this).mousedown( ->
            #jump up to td element and move to first td in this row
            rowData = jQuery(this).parents("tr").data("rowData")
            toAdd = {}
            toAdd[id] = {first: element.first} for id, element of rowData
            contextList[index].addElements(toAdd).show()
          )
        )
      )
      .each((index) ->
        jQuery(this).attr(tabindex: index)
        # add bootstrap class to every button
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
      onshown: (dialog) => @_setQuestionsHandlers(dialog)
    )
    return

  _setQuestionsHandlers: (dialog) =>
    simpleQuestionDialog = new root.SimpleQuestionDialog("getSimpleQuestionDialog/", @_simpleQuestionCreatedHandler)
    sortQuestionDialog = new root.SortQuestionDialog('getSortQuestionDialog/', @_sortQuestionCreatedHandler)
    multipleChoiceQuestionDialog = new root.MultipleChoiceQuestionDialog('getMultipleChoiceQuestionDialog/', @_multipleChoiceQuestionCreatedHandler)
    jQuery("#dialog button").click( -> dialog.close())
    jQuery("#simpleQuestion").click( -> simpleQuestionDialog.show())
    jQuery("#sortQuestion").click( -> sortQuestionDialog.show())
    jQuery("#multipleChoiceQuestion").click( -> multipleChoiceQuestionDialog.show())
    return

  _simpleQuestionCreatedHandler: =>
    name = jQuery("#dialog .form-group:eq(0) input").val()
    question = jQuery("#dialog .form-group:eq(1) input").val()
    answerType = jQuery("#dialog input[type=radio]").first().parent().hasClass("active")
    data =
      name: name
      question: question
      answerType: if answerType then 0 else 1
    @_newEntryRequest(data, "/setSimpleQuestion/", @_setNewQuestion)
    return

  _sortQuestionCreatedHandler: =>
    name = jQuery("#dialog .form-group:eq(0) input").val()
    question = jQuery("#dialog .form-group:eq(1) input").val()
    options = []
    jQuery("#dialog .form-group:last-child .input-group input:not(:last)").each( ->
      options.push jQuery(this).val()
    )
    data =
      name: name
      question: question
      options: options
    @_newEntryRequest(data, "/setSortQuestion/", @_setNewQuestion)
    return

  _multipleChoiceQuestionCreatedHandler: =>
    name = jQuery("#dialog .form-group:eq(0) input").val()
    question = jQuery("#dialog .form-group:eq(1) input").val()
    singleAnswer = jQuery("#dialog input[type=radio]").first().parent().hasClass("active")
    options = []
    jQuery("#dialog .form-group:last-child .input-group input:not(:last)").each( ->
      options.push jQuery(this).val()
    )
    data =
      name: name
      question: question
      options: options
      singleAnswer: singleAnswer
    @_newEntryRequest(data, "/setMultipleChoiceQuestion/", @_setNewQuestion)
    return

  _actionCreatedHandler: =>
    text = jQuery("#dialog input").val()
    data =
      text: text
    @_newEntryRequest(data, "/setAction/", @_setNewAction)
    return

  _newEntryRequest: (data, url, callback) =>
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

  _setNewQuestion: (data) =>
    @_questions.addElements([data])
    toAdd = @_questions.parseElementForThreeColumns(data.questionId)
    @_questionsList.addElements(toAdd).show()
    return

  _setNewAction: (data) =>
    @_actions.addElements([data])
    toAdd = @_actions.parseElementForTwoColumns(data.actionId)
    @_actionsList.addElements(toAdd).show()
    return

jQuery(document).ready( ->
  new Handlers()
)
