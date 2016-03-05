root = exports ? this
root.Actions = class Actions extends root.ExperimentData
  constructor: (_list = []) ->
    super _list

  _init: =>
    super
    @_actionDialog = new root.ActionDialog('getActionDialog/')

  _elementListFormat: (viewId) =>
    text: @_elementsDict[viewId].text
    id: @_elementsDict[viewId].actionId
    isNew: @_elementsDict[viewId].isNew

  showDialog: (viewId, readonly = false) =>
    @_actionDialog.readonly = readonly
    @_actionDialog.bindData(@_elementsDict[viewId]).show()

  _getViewId: (index) => @_orderedList[index].actionId
