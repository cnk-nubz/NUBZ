root = exports ? this
root.QuestionsList = class QuestionsList extends root.ListView
  constructor: (_containerId, _rowFactory) ->
    super _containerId, _rowFactory
    @_listType = "questionsActionsTable"
  generateId: (element) => "#{element.type}$#{element.id}"
