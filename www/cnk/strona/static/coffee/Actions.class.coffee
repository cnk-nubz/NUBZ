root = exports ? this
root.Actions = class Actions
  constructor: (@_list = []) ->
    @_init()

  _init: =>
    @_actionDialog = new root.ActionDialog('getActionDialog/')

  addElements: (elements) =>
    @_list = elements
    @

  parseAllForList: =>
    data = {}
    data[key] = @_parseForList key for key in Object.keys(@_list)
    data

  parseElementForList: (id) =>
    data = {}
    data[id] = @_parseForList(id)
    data

  _parseForList: (id) =>
    text: @_list[id].text
    id: @_list[id].actionId
    isNew: @_list[id].isNew

  showDialog: (id, readonly = false) =>
    @_actionDialog.readonly = readonly is true
    @_actionDialog.bindData(@_list[id]).show()
