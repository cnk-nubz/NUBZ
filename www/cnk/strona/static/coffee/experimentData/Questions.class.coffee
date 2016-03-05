root = exports ? this
root.Questions = class Questions extends root.ExperimentData
  constructor: (_list = []) ->
    super _list

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

  _elementListFormat: (viewId) =>
    name: @_elementsDict[viewId].name
    typeName: @_questionType[@_elementsDict[viewId].type].name
    id: @_elementsDict[viewId].questionId
    isNew: @_elementsDict[viewId].isNew
    type: @_elementsDict[viewId].type

  showDialog: (viewId, readonly = false) =>
    dialog = @_questionType[@_elementsDict[viewId].type].dialog
    dialog.readonly = readonly
    @_questionType[@_elementsDict[viewId].type].dialog.bindData(@_elementsDict[viewId]).show()

  _getViewId: (index) => "#{@_orderedList[index].type}$#{@_orderedList[index].questionId}"
