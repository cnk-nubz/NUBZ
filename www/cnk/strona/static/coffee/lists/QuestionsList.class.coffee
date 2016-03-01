root = exports ? this
root.QuestionsList = class QuestionsList extends root.ListView
  generateId: (element) => "#{element.type}$#{element.id}"
