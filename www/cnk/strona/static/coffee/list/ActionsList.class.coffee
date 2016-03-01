root = exports ? this
root.ActionsList = class ActionsList extends root.ListView
  constructor: (_containerId, _rowFactory) ->
    super _containerId, _rowFactory
    @_listType = "questionsActionsTable"
  generateId: (element) => "#{element.id}"
