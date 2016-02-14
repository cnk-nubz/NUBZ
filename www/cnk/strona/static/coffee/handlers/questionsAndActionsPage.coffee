root = exports ? this
class Handlers
  constructor: ->
    # ===== TODO =====
    # Add myList (or whatever) class for two lists
    # and set communication with them on events
    # ================
    @_setHandlers()

  _setHandlers: =>
    jQuery("#questionList .addElement").on('click', @addNewQuestion)
    new root.ActionDialog("getNewActionDialog/", "#actionList .addElement", @actionCreatedHandler)

  addNewQuestion: =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      dataType: 'json'
      context: this
      url: '/getChooseQuestionTypeDialog/'
      success: (data) =>
        @showChooseQuestionType(data.html)
    )

  showChooseQuestionType: (html) =>
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
         @setQuestionsHandlers(dialog)
    )

  setQuestionsHandlers: (dialog) =>
    new root.SimpleQuestionDialog("getSimpleQuestionDialog/", "#simpleQuestion", @simpleQuestionCreatedHandler)
    new root.SortQuestionDialog('getSortQuestionDialog/', "#sortQuestion", @sortQuestionCreatedHandler)
    new root.MultipleChoiceQuestionDialog('getMultipleChoiceQuestionDialog/', "#multipleChoiceQuestion", @questionCreatedHandler)
    jQuery("#dialog button").click( -> dialog.close())

  simpleQuestionCreatedHandler: =>
    questionName = jQuery("#dialog .form-group:eq(0) input").val()
    question = jQuery("#dialog .form-group:eq(1) input").val()
    answerAsNumber = jQuery("#dialog input[type=radio]").first().parent().hasClass("active")
    data =
      questionName: questionName
      question: question
      answerAsNumber: answerAsNumber
    @newEntryRequest(data, "/setSimpleQuestion/", @setList("#questionList .myTable"))
    return

  sortQuestionCreatedHandler: =>
    questionName = jQuery("#dialog .form-group:eq(0) input").val()
    question = jQuery("#dialog .form-group:eq(1) input").val()
    answers = []
    jQuery("#dialog .form-group:last-child .input-group input").each( ->
      answers.push jQuery(this).val()
    )
    answers.pop()
    data =
      questionName: questionName
      question: question
      answers: answers
    @newEntryRequest(data, "/setSortQuestion/", @setList("#questionList .myTable"))
    return

  multipleChoiceQuestionCreatedHandler: =>
    questionName = jQuery("#dialog .form-group:eq(0) input").val()
    question = jQuery("#dialog .form-group:eq(1) input").val()
    singleAnswerOnly = jQuery("#dialog input[type=radio]").first().parent().hasClass("active")
    answers = []
    jQuery("#dialog .form-group:last-child .input-group input").each( ->
      answers.push jQuery(this).val()
    )
    answers.pop()
    data =
      questionName: questionName
      question: question
      answers: answers
      singleAnswerOnly: singleAnswerOnly
    @newEntryRequest(data, "/setMultipleChoiceQuestion/", @setList("#questionList .myTable"))
    return

  actionCreatedHandler: =>
    actionName = jQuery("#dialog input").val()
    data =
      actionName: actionName
    @newEntryRequest(data, "/setAction/", @setList("#actionList .myTable"))

  newEntryRequest: (data, url, callback) =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      dataType: 'json'
      data: (jsonData: JSON.stringify(data))
      context: this
      url: url
      success: (data) =>
        callback(data.list)
    )

  setList: (container) =>
    (list) =>
      for entry in list
        obj = jQuery("<tr><td><div>#{entry}</div></td></tr>")
        jQuery(obj).appendTo(container)
        obj.children().children().shortenText()
      return

jQuery(document).ready( ->
  handlers = new Handlers()
)
