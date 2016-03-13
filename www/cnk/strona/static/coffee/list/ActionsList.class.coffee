root = exports ? this
root.ActionsList = class ActionsList extends root.ListView
  constructor: ->
    super
  generateId: (element) => "#{element.id}"

  getAllElements: =>
    (+el for el, status of @_elementsOnList when status is true)
