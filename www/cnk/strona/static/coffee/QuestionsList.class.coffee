root = exports ? this
root.QuestionsList = class QuestionsList extends root.View
  constructor: (@_containerId) ->
    super
    @_questions = []
    @_init()

  _init: =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      context: this
      url: '/getQuestionsList/'
      success: (data) =>
        jQuery(data).appendTo(@_containerId)
        @_getQuestionsListElementHTML()
    )

  _getQuestionsListElementHTML: =>
    jQuery.ajaxSetup(
      headers: { "X-CSRFToken": getCookie("csrftoken") }
    )
    jQuery.ajax(
      type: 'POST'
      context: this
      url: '/getQuestionsListElement/'
      success: (data) ->
        @_questionsListElementHTML = data
        # TODO CHANGE FROM FAKE LIST TO REAL QUESTIONS:
        qList = {}
        qList[0] = {
          id: 2
          name: "nazwa pierwszego"
          type: "otwarte"
        }
        qList[1] = {
          id: 5
          name: "nazwa drugiego pytania nazwa drugiego pytania nazwa drugiego pytania nazwa drugiego pytania"
          type: "zamknięte"
        }
        qList[2] = {
          id: 7
          name: "pytanie z sortowaniem"
          type: "sortowanie"
        }
        @addQuestions(qList)
    )
    return

  addQuestions: (questions) =>
    for q of questions
      questionListElement = jQuery(@_questionsListElementHTML)
      questionName = jQuery(".questionNameCell > div", questionListElement)
      questionBadge = jQuery(".questionBadge", questionListElement)
      questionListElement.data("name", questionName)
        .data("badge", questionBadge)
      questionName.html "#{questions[q].name}"
      questionBadge.html questions[q].type
      id = questions[q].id
      jQuery(questionListElement).click( do (id) => (=>
        console.log("clicked #{id}")
        @fireEvents("questionClicked", id)))
      @_questions.push questionListElement
    @_refreshQuestionsList()

  _refreshQuestionsList: =>
    jQuery("#questionsList .questionsListElement").each( -> jQuery(this).remove())
    for q in @_questions
      question = q.clone(true, true)
      jQuery(question).appendTo("#questionsListTable > tbody")
      jQuery(".questionNameCell > div", question).shortenText()
    return
