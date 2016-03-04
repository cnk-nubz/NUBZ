root = exports ? this
root.QuestionsList = class QuestionsList extends root.ListView
  constructor: ->
    @_listType = "questionsActionsTable"
    super
  generateId: (element) => "#{element.type}$#{element.id}"
