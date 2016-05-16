root = exports ? this
root.Actions = class Actions extends root.ExperimentData
  ###
  # type Action = {
  #   actionId :: Int,
  #   isNew    :: Boolean,
  #   text     :: String
  # }
  ###
  # constructor :: [Action] -> Context
  constructor: (_list = []) ->
    super _list
    new root.ActionDialog()


  ###
  # type ActionListElement = {
  #   text  :: String,
  #   id    :: Int,
  #   isNew :: Boolean
  # }
  ###
  # _elementListFormat :: String -> ActionListElement
  _elementListFormat: (viewId) =>
    text: @escapeText(@_elementsDict[viewId].text)
    id: @_elementsDict[viewId].actionId
    isNew: @_elementsDict[viewId].isNew


  # getFilledDialog :: (String, jsObject) -> BootstrapDialog
  getFilledDialog: (viewId, options = {}) =>
    dialog = new root.ActionDialog('getHTML?name=actionDialog', options)
    dialog.bindData(@_elementsDict[viewId])
    dialog


  # _getViewId :: Int -> String
  _getViewId: (index) =>
    JSON.stringify(
      id: @_orderedList[index].actionId
    )
