root = exports ? this
class Handlers
  constructor: ->
    @_setButtonHandlers()

  _setButtonHandlers: =>
    new root.SimpleQuestionDialog("getSimpleQuestionDialog/", "#simpleQuestion")
    new root.SortQuestionDialog('getSortQuestionDialog/', "#sortQuestion")
    new root.MultipleChoiceQuestionDialog('getMultipleChoiceQuestionDialog/', "#multipleChoiceQuestion")
    new root.ActionDialog("getNewActionDialog/", "#newAction")

jQuery(document).ready( ->
  new Handlers()
)
