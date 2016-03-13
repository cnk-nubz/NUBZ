root = exports ? this
root.QuestionsList = class QuestionsList extends root.ListView
  constructor: ->
    super

  getAllElements: =>
    elements = (JSON.parse(el) for el in @_elementsOnList)
    result =
      questionsOrder: (el.type for el in elements)
      simpleQuestions: (el.id for el in elements when el.type is 0)
      multipleChoiceQuestions: (el.id for el in elements when el.type is 1)
      sortQuestions: (el.id for el in elements when el.type is 2)
    result
