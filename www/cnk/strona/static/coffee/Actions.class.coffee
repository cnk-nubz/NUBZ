root = exports ? this
root.Actions = class Actions
  constructor: (@_list = []) ->
    @_init()

  _init: =>
    @_actionDialog = new root.ActionDialog('getActionDialog/')

  setElements: (elements) =>
    @_list = elements
    @

  listFormat: =>
    data = {}
    data[key] = @_elementListFormat key for key in Object.keys(@_list)
    data

  _elementListFormat: (id) =>
    text: @_list[id].text
    id: @_list[id].actionId
    isNew: @_list[id].isNew

  showDialog: (id, readonly = false) =>
    @_actionDialog.readonly = readonly
    @_actionDialog.bindData(@_list[id]).show()
