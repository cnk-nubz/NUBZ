root = exports ? this
root.QuestionsList = class QuestionsList extends root.ListView
  constructor: ->
    super
  generateId: (element) => "#{element.type}$#{element.id}"
