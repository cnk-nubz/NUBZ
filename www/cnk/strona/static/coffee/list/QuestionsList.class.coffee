root = exports ? this
root.QuestionsList = class QuestionsList extends root.ListView
  constructor: ->
    super
  generateId: (element) => "#{element.type}$#{element.id}"

  getAllElements: =>
    elements = (el for el, status of @_elementsOnList when status is true)
    result =
      questionsOrder: (+el.split("$")[0] for el in elements)
      simpleQuestions: (+el.split("$")[1] for el in elements when +el.split("$")[0] is 0)
      multipleChoiceQuestions: (+el.split("$")[1] for el in elements when +el.split("$")[0] is 1)
      sortQuestions: (+el.split("$")[1] for el in elements when +el.split("$")[0] is 2)
    result
