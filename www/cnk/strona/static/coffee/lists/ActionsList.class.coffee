root = exports ? this
root.ActionsList = class ActionsList extends root.ListView
  generateId: (element) => "#{element.id}"
