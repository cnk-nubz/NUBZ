root = exports ? this
root.ActionsList = class ActionsList extends root.ListView
  constructor: ->
    super
  generateId: (element) => "#{element.id}"
