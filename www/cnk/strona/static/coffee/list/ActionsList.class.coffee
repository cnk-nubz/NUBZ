root = exports ? this
root.ActionsList = class ActionsList extends root.ListView
  constructor: ->
    super

  getAllElements: =>
    (JSON.parse(el).id for el in @_elementsOnList)
