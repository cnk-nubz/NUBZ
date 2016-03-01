root = exports ? this
root.Questions = class Questions
  constructor: (@_list = []) ->
    @_init()

  _init: =>
    @_simpleQuestionDialog = new root.SimpleQuestionDialog('getSimpleQuestionDialog/')
    @_multipleChoiceQuestionDialog = new root.MultipleChoiceQuestionDialog('getMultipleChoiceQuestionDialog/')
    @_sortQuestionDialog = new root.SortQuestionDialog('getSortQuestionDialog/')
    @_questionType =
      0:
        name: 'otwarte'
        dialog: @_simpleQuestionDialog
      1:
        name: 'zamknięte'
        dialog: @_multipleChoiceQuestionDialog
      2:
        name: 'sortowanie'
        dialog: @_sortQuestionDialog

  setElements: (elements) =>
    @_list = elements
    @

  listFormat: =>
    data = {}
    data[key] = @_parseForList key for key in Object.keys(@_list)
    data

  _parseForList: (id) =>
    name: @_list[id].name
    typeName: @_questionType[@_list[id].type].name
    id: @_list[id].questionId
    isNew: @_list[id].isNew
    type: @_list[id].type

  showDialog: (id, readonly = false) =>
    dialog = @_questionType[@_list[id].type].dialog
    dialog.readonly = readonly is true
    @_questionType[@_list[id].type].dialog.bindData(@_list[id]).show()
