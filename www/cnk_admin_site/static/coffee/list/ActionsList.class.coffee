root = exports ? this
root.ActionsList = class ActionsList extends root.ListView
  # getAllElements :: () -> [Int]
  getAllElements: =>
    (JSON.parse(el).id for el in @_elementsOnList)
