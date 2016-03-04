root = exports ? this
root.ActionsList = class ActionsList extends root.ListView
  constructor: ->
    @_listType = "questionsActionsTable"
    super
  generateId: (element) => "#{element.id}"
