root = exports ? this
root.Actions = class Actions extends root.ExperimentData
  constructor: (@_list = []) ->
    super

  _init: =>
    super
    @_actionDialog = new root.ActionDialog('getActionDialog/')

  _elementListFormat: (id) =>
    text: @_elements[id].text
    id: @_elements[id].actionId
    isNew: @_elements[id].isNew

  showDialog: (id, readonly = false) =>
    @_actionDialog.readonly = readonly
    @_actionDialog.bindData(@_elements[id]).show()

  getRealId: (index) => @_list[index].actionId
