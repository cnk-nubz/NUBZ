root = exports ? this
root.Questions = class Questions extends root.ExperimentData
  constructor: (@_list = []) ->
    super

  _init: =>
    super
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

  _elementListFormat: (id) =>
    name: @_elements[id].name
    typeName: @_questionType[@_elements[id].type].name
    id: @_elements[id].questionId
    isNew: @_elements[id].isNew
    type: @_elements[id].type

  showDialog: (id, readonly = false) =>
    dialog = @_questionType[@_elements[id].type].dialog
    dialog.readonly = readonly is true
    @_questionType[@_elements[id].type].dialog.bindData(@_elements[id]).show()

  getRealId: (index) => "#{@_list[index].type}$#{@_list[index].questionId}"
