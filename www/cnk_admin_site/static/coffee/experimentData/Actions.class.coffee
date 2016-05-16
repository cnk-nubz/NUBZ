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
    @_actionDialog = new root.ActionDialog('getHTML?name=actionDialog')


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


  # showDialog :: (String, Boolean) -> Context
  showDialog: (viewId, readonly = false) =>
    @_actionDialog.readonly = readonly
    @_actionDialog.bindData(@_elementsDict[viewId]).show()
    @


  # _getViewId :: Int -> String
  _getViewId: (index) =>
    JSON.stringify(
      id: @_orderedList[index].actionId
    )
